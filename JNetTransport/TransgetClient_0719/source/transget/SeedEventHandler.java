package transget;
import java.io.File;
import java.util.ArrayList;

public class SeedEventHandler implements SeedEventModel {
    
    /** array duplicate of runtime */
    private ArrayList array;
    /** array locak duplicate of runtime */
    private Object key;

    public SeedEventHandler(ArrayList a, Object lock) {
      this.array = a;
      this.key = lock;
    }

    public void removeFinish() {
      synchronized(key) {
	for (int i=(array.size()-1); i >= 0; i--) {
	  Seed seed = (Seed) array.get(i);
	  if (seed.getStatus() == Seed.FINISH) array.remove(i);
	}
      }
    }

    public void removeSeedRange(int[] rows) {
      synchronized(key) {
	for (int i=(rows.length-1); i>=0 ; i--) {
	  Seed seed = (Seed) array.get(rows[i]);
	  if (seed.getStatus() == Seed.RUN) seed.stop();
	  array.remove(rows[i]);
	}
      }
    }
     
    public void removeAllSeed() {
      synchronized(key) {
	for (int i=(array.size()-1); i >= 0; i--) {
	  Seed seed = (Seed) array.get(i);
	  if (seed.getStatus() == Seed.RUN) seed.stop();
	  array.remove(i);
	}
      }
    }

    public void startSeedRange(int[] rows) {
      synchronized(key) {
	for (int i=(rows.length-1); i>=0 ; i--) {
	  Seed seed = (Seed) array.get(rows[i]);
	  if (seed.getStatus() == Seed.STOP)
	    seed.setStatus(Seed.QUEUE);
	}
      }
    }
    
    public void startAllSeed() {
      synchronized(key) {
	for (int i=(array.size()-1); i >= 0; i--) {
	  Seed seed = (Seed) array.get(i);
	  if (seed.getStatus() == Seed.STOP) {
	    seed.setStatus(Seed.QUEUE);
	  }
	}
      }
    }

    /**
     *  if seed is run set to STOP
     *  and interrupt the running thread
     */
    public void stopSeedRange(final int[] rows) {
      synchronized(key) {
	for (int i=(rows.length-1); i>=0 ; i--) {
	  Seed seed = (Seed) array.get(rows[i]);
	  //if (seed.getStatus() == Seed.RUN
	//	|| seed.getStatus() == Seed.QUEUE) {
	  //  if (seed.getStatus() == Seed.RUN) seed.stop();
	    seed.stop();
	    seed.setStatus(Seed.STOP);
	  //}
	}
      }
    }

    public void stopAllSeed() {
      synchronized(key) {
	for (int i=(array.size()-1); i >= 0; i--) {
	  Seed seed = (Seed) array.get(i);
	  if (seed.getStatus() == Seed.RUN ||
		seed.getStatus() == Seed.QUEUE) {
	    if (seed.getStatus() == Seed.RUN) seed.stop();
	    seed.setStatus(Seed.STOP);
	  }
	}
      }
    }


    public void removeSeedFile(int[] rows) {
      synchronized(key) {
	for (int i=(rows.length-1); i>=0 ; i--) {
	  Seed seed = (Seed) array.get(rows[i]);
	    File file = new File(seed.getFileStatus().saveDir,
				seed.getFileStatus().fileName);
	  if (file.exists())
	  if (!file.delete())
	    System.out.println(file.toString()+" can't delete.");

	  seed.getFileStatus().init(false);
	}
      }
    }
    
    public void removeErrorsFile() {
      synchronized(key) {
	for (int i=(array.size()-1); i>=0 ; i--) {
	  Seed seed = (Seed) array.get(i);
	  if (seed.getStatus() == Seed.PARTIAL) {
		File file = new File(seed.getFileStatus().saveDir,
				seed.getFileStatus().fileName);
	    if (file.exists())
	    if (!file.delete())
		System.out.println(file.toString()+" can't delete.");
	    seed.getFileStatus().init(false);
	  }
	}
      }
    }

    public void setSeedZeroRange(int[] rows) {
      synchronized(key) {
	for (int i=(rows.length-1); i>=0; i--) {
	  Seed seed = (Seed) array.get(rows[i]);
	  if (seed.getStatus() != Seed.RUN) {
	    int flag = seed.getStatus();
	    seed.setValueToDefault();
	    if (flag == Seed.FINISH)
	      seed.setStatus(Seed.STOP);
	    else
	      seed.setStatus(flag);
	  }
	}
      }
    }

    public void setSeedZeroAll() {
      synchronized(key) {
	for (int i=(array.size()-1); i>=0; i--) {
	  Seed seed = (Seed) array.get(i);
	  if (seed.getStatus() != Seed.RUN) {
	    int flag = seed.getStatus();
	    seed.setValueToDefault();
	    if (flag == Seed.FINISH)
	      seed.setStatus(Seed.STOP);
	    else
	      seed.setStatus(flag);
	  }
	}
      }
    }
    
    public void setErrorSeedZero() {
      synchronized(key) {
	for (int i=(array.size()-1); i>=0; i--) {
	  Seed seed = (Seed) array.get(i);
	  if (seed.getStatus() == Seed.PARTIAL) {
	    seed.setValueToDefault();
	    seed.setStatus(Seed.STOP);
	  }
	}
      }
    }
    
    public void updateSeed(int index,String dir,String  referer, 
			int delay,int maxtry,int status,
			String fileName,int buffer) {
      synchronized(key) {
	Seed seed = (Seed) array.get(index);
	((SeedHttpURL)seed.getURL()).setReferer(referer);
	seed.setStatus(status);
	seed.setDelay(delay);
	seed.setMaxtry(maxtry);
	seed.setBuffer(buffer);
	SeedFileStatus s = seed.getFileStatus();
	s.saveDir = dir;
	s.setFileName(fileName);
      }
    }
    
    public void updateSeedSaveDir(int index, String newdir) {
      synchronized(key) {
        Seed seed = (Seed) array.get(index);
	SeedFileStatus s = seed.getFileStatus();
	s.saveDir = newdir;
      }
    }

    public void updateSeedReferer(int index, String referer) {
      synchronized(key) {
	Seed seed = (Seed) array.get(index);
	((SeedHttpURL)seed.getURL()).setReferer(referer);
      }
    }

    public void updateSeedDelay(int index, int delay) {
      synchronized(key) {
	Seed seed = (Seed) array.get(index);
	seed.setDelay(delay);
      }
    }

    public void updateSeedMaxtry(int index, int maxtry) {
      synchronized(key) {
	Seed seed = (Seed) array.get(index);
	seed.setMaxtry(maxtry);
      }
    }
}
