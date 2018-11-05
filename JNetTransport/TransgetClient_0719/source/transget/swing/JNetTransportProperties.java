package transget.swing;
import transget.*;
import java.awt.Toolkit;
import java.util.Properties;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.IOException;
import java.io.Serializable;

public class JNetTransportProperties {

    static String version = "д╗  4ды 10 21:46:13 CST 2004";

    /**
     *  UI properties
     */
    
    static int frame_width = 
	    Toolkit.getDefaultToolkit().getScreenSize().width-200;
    static int frame_height = 
	    Toolkit.getDefaultToolkit().getScreenSize().height-140;
    static int locationX = 
	    (Toolkit.getDefaultToolkit().getScreenSize().width-
	    (Toolkit.getDefaultToolkit().getScreenSize().width-200))/2;
    static int locationY = 
	    (Toolkit.getDefaultToolkit().getScreenSize().height-
	    (Toolkit.getDefaultToolkit().getScreenSize().height-140))/2; 
    static String frame_theme =
	    "com.incors.plaf.kunststoff.KunststoffLookAndFeel";
    
    static String font_face = "PMingLiU";
    static int small_size = 12;
    static int middle_size = 13;
    static int big_size = 16;

    /**
     *  Seed properties
     */   
 
    static String savePath = System.getProperty("user.home");
    static int delay = 2000;
    static int maxtry = 50;
    static int buffer = 90000; // 900 KB
    static int maxstream = 3;

    /**
     *	frame.width=
     *	frame.height=
     *	frame.locationX=
     *	frame.locationY=
     *	frame.theme=
     *	seed.savepath=
     *	seed.delay=
     *	seed.maxtry=
     *	seed.buffer=
     *	seed.maxstream=
     */

    public JNetTransportProperties() {
      readProperties();
    }
    
    public void readProperties() {
      java.io.File file = new java.io.File("jnettransport.properties");
      if (!file.exists()) return;
      try {
	Properties p = new Properties();
	p.load(new FileInputStream(file));
	frame_theme =
	    p.getProperty("frame.theme");
	frame_width =
	    Integer.parseInt(p.getProperty("frame.width"));
	frame_height =
	    Integer.parseInt(p.getProperty("frame.height"));
	locationX =
	    Integer.parseInt(p.getProperty("frame.locationX"));
	locationY =
	    Integer.parseInt(p.getProperty("frame.locationY"));
	font_face = 
	    p.getProperty("font.face");
	small_size = 
	    Integer.parseInt(p.getProperty("font.smallSize"));
	middle_size = 
	    Integer.parseInt(p.getProperty("font.middleSize"));
	big_size = 
	    Integer.parseInt(p.getProperty("font.bigSize"));
	savePath = 
	    p.getProperty("seed.savepath");
	maxtry =
	    Integer.parseInt(p.getProperty("seed.maxtry"));
	delay =
	    Integer.parseInt(p.getProperty("seed.delay"));
	buffer =
	    Integer.parseInt(p.getProperty("seed.buffer"));
	maxstream =
	    Integer.parseInt(p.getProperty("seed.maxstream"));
	p = null;	
      } catch (IOException ioe) {;}
    }
    
    public void writeProperties() {
      try {
	Properties p = new Properties();
	p.setProperty("frame.theme",String.valueOf(frame_theme));
	p.setProperty("frame.width",String.valueOf(frame_width));
	p.setProperty("frame.height",String.valueOf(frame_height));
	p.setProperty("frame.locationX",String.valueOf(locationX));
	p.setProperty("frame.locationY",String.valueOf(locationY));
	p.setProperty("font.face",font_face);
	p.setProperty("font.smallSize",String.valueOf(small_size));
	p.setProperty("font.middleSize",String.valueOf(middle_size));
	p.setProperty("font.bigSize",String.valueOf(big_size));
	p.setProperty("seed.savepath",savePath);
	p.setProperty("seed.maxtry",String.valueOf(maxtry));
	p.setProperty("seed.delay",String.valueOf(delay));
	p.setProperty("seed.buffer",String.valueOf(buffer));
	p.setProperty("seed.maxstream",String.valueOf(maxstream));
	p.store(
	    new FileOutputStream(new File("jnettransport.properties")),null);
	p = null;	
      } catch (IOException ioe) {;}
    }
    
    public void readSeedURL(RuntimeThread runtime, SeedPoolStatus v) {
      File file = new File("seedp.ser");
      if (!file.exists()) return;
      try {
	  FileInputStream fis = new FileInputStream(file);
	  ObjectInputStream obj = new ObjectInputStream(fis);
	  SeedProperty p = null;
	  while(true) {
	    try {
	    p = (SeedProperty)obj.readObject();
	    } catch (Exception e) { break; }

      Seed seed = Seed.newInstance(p.url,p.fileName,p.saveDir,Seed.STOP,p.maxtry,p.delay,p.buffer);
      if (seed.getURL() instanceof SeedHttpURL) {
	if (p.referer != null)
	  if (!p.referer.equals(""))
	    ((SeedHttpURL)seed.getURL()).setReferer(p.referer);
	    runtime.add(seed);
      }

	    p = null;
	  }
	  v.fireTable();
	  obj.close(); obj = null; fis = null; file = null;
      } catch (Exception e){ e.printStackTrace(); }
    }
    
    public void writeSeedURL(RuntimeThread runtime) {
      File file = new File("seedp.ser");
      try {
	FileOutputStream fos = new FileOutputStream(file);
	ObjectOutputStream obj = new ObjectOutputStream(fos);
	for (int i=0; i<runtime.getSize(); i++) {
	    Seed seed = (Seed) runtime.get(i);
	    SeedFileStatus s = seed.getFileStatus();
	    SeedProperty seedp = new SeedProperty();
	    seedp.url = seed.getURL().getURL();
	    seedp.referer = ((SeedHttpURL)seed.getURL()).getReferer();
	    seedp.fileName = s.fileName;
	    seedp.saveDir = s.saveDir;
	    seedp.delay = seed.getDelay();
	    seedp.maxtry = seed.getMaxtry();
	    seedp.buffer = seed.getBuffer();
	    obj.writeObject(seedp);
	}
	obj.flush(); obj.close(); obj = null; fos = null; file = null;
      } catch (Exception e){ e.printStackTrace(); }
    }


}
class SeedProperty implements Serializable {
	String url;
	String referer;
	String fileName;
	String saveDir;
	int delay;
	int maxtry;
	int buffer;
    }

