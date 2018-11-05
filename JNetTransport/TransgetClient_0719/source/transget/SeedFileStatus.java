package transget;
import java.io.*;

public class SeedFileStatus {
    /* fileName */
    public String fileName;
    /* saveas */
    public String saveDir;
    /* exist file size */
    public long fileSize;
    /* remote file size */
    public long contentSize;
    /* reads */
    public long reads;
    /* arm for speed */
    public int oldreads;
    /* speed */
    public int speed;
    /* speed array */
    public int speedAry[] = new int[2];
    /* spend time */
    public int time;
    /* time left */
    public int timeLeft;
    /* percent */
    public int percent;

    /* speed  label*/
    public String speedLabel;
    /* time label */
    public String timeLabel;
    /* time left label */
    public String timeLeftLabel;

    public SeedFileStatus(String dir,String name) {
      saveDir = dir;
      fileName = name;
      init(false);
    }

    public void init(boolean keepTimeLabel) {
      File file = new File(saveDir, fileName);
      if (file.exists()) {
	try {
	  FileInputStream fis = new FileInputStream(file);
	  this.fileSize = fis.available();
	  this.reads = fis.available();
	  fis.close(); fis = null;
	  file = null;
	} catch (IOException ioe) {;}
      } else {
	this.fileSize = 0;
	this.reads = 0;
      }
      if (!keepTimeLabel)
	this.timeLabel = "00:00:00";
      this.timeLeftLabel = "00:00:00";
      this.speedLabel = "0";
      if (reads > 0 && contentSize > 0)
      this.percent = new Double(
		    ((double)reads/(double)contentSize)*100).intValue();
    }

    public void setDir(String dir) {
      this.saveDir = dir;
      init(true);
    }
    
    public void setFileName(String fileName) {
      if (fileName.equals("")) return;
      this.fileName = fileName;
      init(true);
    }
}
