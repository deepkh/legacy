package transget;
import java.util.ArrayList;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.Observer;

public class Seed {

    public static int increase = 0;
    
    public final static int QUEUE = 0x1;
    public final static int STOP = 0x2;
    public final static int RUN = 0x3;
    public final static int FINISH = 0x4;
    public final static int PARTIAL = 0x5;
    /**
     * seed status , may be WAIT ,RUN or QUERE
     */
    protected int status;
    /* clone for status. */
    protected int origin;
    /* Seed Index */
    private int identify;

    /* no content size, try time */
    public int noContentSizeTryed = 30;
    
    /* try times */
    private int tryed;
    /* delay millens */
    private int delay;
    /* max try time */
    private int maxtry;
    /* buffer size */
    private int buffer;
    
    /* seed's url warraper */
    private SeedURL url;
    /* seed transport */
    private SeedTransport transport;
    /* seed's ui status */
    private SeedFileStatus fileStatus;
    /* seed's logger */
    private SeedLogger logger;

    protected Seed(int id,SeedURL url,SeedFileStatus status,SeedLogger logger) {
      this.identify = id;
      this.url = url;
      this.fileStatus = status;
      this.logger = logger;
    }

    public static Seed newInstance(String u,String fileName,String dir,
			    int status,int maxtry,int delay,int buffer) {
      SeedURL url = SeedURL.newInstance(u);
      if (url == null) return null;
      if (url instanceof SeedHttpURL) {
	SeedLogger logger = new SeedLogger();
	if (fileName == null)
	  fileName = url.getFileName();
	else if (fileName.equals(""))
	  fileName = url.getFileName();
	SeedFileStatus fileStatus = new SeedFileStatus(dir,fileName);
	((SeedHttpURL) url).setRange(fileStatus.fileSize);
	Seed seed = new Seed(++increase,url,fileStatus,logger);
	seed.setStatus(status);
	seed.setMaxtry(maxtry);
	seed.setDelay(delay);
	seed.setBuffer(buffer);
	return seed;
      }
      return null;
    }

    public SeedURL getURL() {
      return url;
    }

    public void start(Observer obs) {
      try {
	Class cls = Class.forName(url.getTransportClass());
	System.out.println("class: "+url.getTransportClass());
	transport = (SeedHttpTransport) cls.newInstance();
	transport.init(this);
	transport.addObserver(obs);
	transport.start();
      } catch (Exception e) { e.printStackTrace();}
    }	 

    public void stop() {
      if (transport != null) {
	transport.stop();
	transport = null;
      }
    }
    
    public SeedFileStatus getFileStatus() {
      return fileStatus;
    }

   public SeedLogger getLogger() {
      return logger;
    }

    /**
     *
     *  status, id, maxtry, tryed, delay, buffer 
     */
    public void setStatus(int type) {
      this.status = type;
    }

    public int getStatus() {
      return status;
    }

    public int getIdentify() {
      return identify;
    }
    
    public void setMaxtry(int m) {
      this.maxtry = m;
    }

    public int getMaxtry() {
      return maxtry;
    }

    public void setTryed(int i) {
      this.tryed = i;
    }

    public int getTryed() {
      return tryed;
    }

    public void setDelay(int d) {
      this.delay = d;
    }

    public int getDelay() {
      return delay;
    }

    public void setBuffer(int b) { 
      this.buffer = b;
    }

    public int getBuffer() {
      return buffer;
    }

    public void setValueToDefault() {
      this.status = STOP;
      this.maxtry = 30;
      this.tryed = 0;
      this.noContentSizeTryed = 30;
    }

    public boolean equal(Seed seed) {
      return (this.identify == seed.getIdentify());
    }

}
