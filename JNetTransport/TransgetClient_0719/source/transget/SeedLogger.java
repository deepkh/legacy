package transget;
import java.util.ArrayList;

public class SeedLogger {
    /* log msg */
    protected ArrayList logger;

    public SeedLogger() {
      logger = new ArrayList();
    }

    /** green */
    public void logSeedPutMsg(String msg) {
      logger(1,msg);
    }
    
    /** green */
    public void logSeedGetMsg(String msg) {
      logger(2,msg);
    }
    
    /** blue */
    public void logSeedWarningMsg(String msg) {
      logger(3,msg);
    }
    
    /** red */
    public void logSeedErrorMsg(String msg) {
      logger(4,msg);
    }

    /**
      *  1*2004-03-05 pm: 213*MSG
      */
    protected void logger(int type, String msg) {
      SeedLogMsg log = new SeedLogMsg(type, msg);
      logger.add(log);
    }

    public int size() {
      return logger.size();
    }

    public synchronized SeedLogMsg get(int i) {
      return (SeedLogMsg) logger.get(i);
    }
}
