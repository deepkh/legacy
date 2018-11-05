package transget;
import java.io.File;
import java.util.ArrayList;
import java.util.Vector;
import java.util.Enumeration;
import java.util.Observer;
import java.util.Observable;

public class RuntimeThread implements Runnable,Observer {
    /* RuntimeThread's Thread */
    private Thread thread;
    /* Monitor's Thread */
    private Thread mthread;
    /* Seed's Monitor */
    private SeedMonitor seedMonitor;
    /* monitor's pool */
    protected Vector monitor;
    /* Seed's Pool */
    private ArrayList array;
    /* key lock */
    private Object key;
    /* GUI's status  */
    //public SeedStatusViewer status;
    public SeedStatusUpdater updater;
    /* Seed Pool Monitor */
    public SeedPoolStatus status;
    /* max socket stream thread */
    protected int max = 3;
    /* auto remove completed */
    protected boolean autoRemoveCompleted;
    public boolean sleep;
    public boolean running = true;

    protected SeedEventHandler handler;
    
    public RuntimeThread(SeedStatusUpdater updater) {
      this.status = new SeedPoolStatus(this, updater);
      this.updater = updater;
      this.key = new Object();
      this.array = new ArrayList();
      this.monitor = new Vector();
      this.handler = new SeedEventHandler(array, key);
      this.seedMonitor = new SeedMonitor();
      if (thread == null) {
	thread = new Thread(this);
	mthread = new Thread(seedMonitor);
	thread.start();
	mthread.start();
      }
    }

    public void run() {
      while(running) {
	START_CONNECT :
	synchronized(key) {
	  for (int i=0; i<array.size(); i++) {
	    Seed seed = (Seed) array.get(i);
	    if (seed.getStatus() == Seed.QUEUE) {
	      if (monitor.size() < max) {
		seed.start(this);
		monitor.add(seed);
		if (seedMonitor.isSleep())
		  seedMonitor.wakeup();
	      } else {
		break START_CONNECT;
	      }
	    } else {
	      continue;
	    }
	  }
	}
      /*
	START_CONNECT :
	for (Enumeration e=pool.elements();e.hasMoreElements();) {
	  Seed seed = (Seed) e.nextElement();
	  if (seed.getStatus() == Seed.QUEUE) {
	    if (monitor.size() < max) {
	      seed.start(this);
	      monitor.add(seed);
	      if (seedMonitor.isSleep())
		seedMonitor.wakeup();
	    } else {
	      break START_CONNECT;
	    }
	  } else {
	    continue;
	  }
	}
*/
	try {
	  synchronized(this) {
	    this.sleep = true;
	    this.wait();
	  }
	} catch (InterruptedException e) {
	  this.sleep = false;    
	}
      }
    }


    /**
     * Seed Array Function
     * arm for viewer use
     */

    public void add(Seed seed) {
      synchronized(key) {
	array.add(seed);
      }
    }

    public Seed get(int index) {
      synchronized(key) {
	return (Seed) array.get(index);
      }
    }

    public int getSize() {
      synchronized(key) {
	return array.size();
      }
    }

    public int findId(Seed seed) {
	synchronized(key) {
	    for (int i=0; i<array.size(); i++)
		if (seed.getIdentify() == get(i).getIdentify())
		return i;
	    return 0;
	}
    }

    public Object getSyncKey() {
	return key;
    }

    public SeedPoolStatus getStatus() {
	return status;
    }

    /**
     *  Runtime Thread Function
     */

    public void setMaxStream(int max) {
      this.max = max;
      wakeup();
    }
    
    public synchronized void wakeup() {
      if (sleep) this.notifyAll();
    }

    public SeedEventHandler getEventHandler() {
      return this.handler;
    }



    /**
     *  Child thread, Transport Function
     *  remove it from monitor vector
     */
     
    public synchronized void update(Observable obj,Object arg) {
	synchronized(key) {
      SeedTransport trans = (SeedTransport) obj;
      Seed seed = trans.getSeed();
      monitor.remove(seed);
      wakeup();
      Seed sed = null;
      status.updateTableRange(findId(seed));
      status.updateFileNum();
      status.updateFileSize();
      }
      /*
      int id = seed.getIdentify();
      for (int i=0; i<array.size(); i++) {
	if ((sed = get(i)).getIdentify() == id) {
	    status.updateTableRange(id);
	}
      }
      */
    }
    
    
    class SeedMonitor implements Runnable {
	public boolean sleep;
	public String makeTimeLabel(int time) {
	  String lab = "";
	  int sec = 0, min = 0, hour = 0;
	  if (time >= 3600) { hour = time/3600;
			  time = time%3600; }
	  if (time >= 60) { min = time/60; }
	  sec = time%60;

	  if (hour < 10) { lab = "0"+hour+":"; }
	  else { lab = hour+":"; }

	  if (min < 10) { lab += "0"+min+":";}
	  else {lab += min+":";}

	  if (sec < 10) { lab += "0"+sec;}
	  else {lab += sec;}

	  return lab;
	}
	
	public void countFully(Seed seed, int index) {
	  /**
	    * 1. count speedArray
	    * 2. set oldreads = reads
	    * 3. count speed array
	    * 3. count speed 
	    * 3. count speed label
	    *   reads && content > 0
	    * 4. count percent
	    * 5. count time
	  */
	  SeedFileStatus s = seed.getFileStatus();
	  long reads = s.reads;
	  long contentSize = s.contentSize;
	  int current = 0;
	  if (s.oldreads == 0) current = (int)s.reads;
	  else current = ((int)s.reads) - s.oldreads;
	  s.oldreads = (int)reads;
	  s.speedAry[index] = current;
	  if (index == 0) {
	    s.time += 1;
	    s.speed = (s.speedAry[0]+s.speedAry[1]);
	    int speed = s.speed;
	    s.speedLabel = (speed>1024?(speed/1024)+"KBs":speed+"Bs");
	    if (reads>0 && contentSize >0) {
	      s.percent = new Double(((double)reads/(double)contentSize)*100).intValue();
	      int leftSize = (int)(contentSize-reads);
	      if (speed > 0)
		s.timeLeft = leftSize/speed;
	      s.timeLabel = makeTimeLabel(s.time);
	      s.timeLeftLabel = makeTimeLabel(s.timeLeft);
	    }
	  }
	}

	public void run() {
	  int increse = 1,  totalspeed = 0;
	  while(running) {
	    while(monitor.size() > 0) {
	      totalspeed = 0;
	      int min = 0, max = 0;
		//synchronized(observer) {
	      try {
		for (int i=0; i<monitor.size(); i++) {
		  Seed seed = (Seed) monitor.elementAt(i);
		  if (i == 0)
		    min = findId(seed);
		  else if (i == (monitor.size()-1))
		    max = findId(seed);
		  countFully(seed, increse%2);
		  totalspeed += seed.getFileStatus().speed;
		}
	      } catch (java.lang.ArrayIndexOutOfBoundsException e) {;}
	    //}

//		System.out.println(min+"/"+max);
	      increse++;
	      String speed = (totalspeed>1024?(totalspeed/1024)+"KBs"
					    :totalspeed+"Bs");
	      //status.updateRunningSeed(speed);
	      status.updateSpeed(speed);
	      status.updateTableRange(new int[]{min, max});
	      try {
		  Thread.sleep(500);
	      } catch (InterruptedException e) {;}
	    }
	    
	    try {
	      synchronized(this) {
		sleep=true;
		System.out.println("mthread:\t\t\twaitting");
		this.wait();
	      }
	    } catch (InterruptedException e) {sleep=false;}

	  }
	}

	public boolean isSleep() {
	  return sleep;
	}

	public synchronized void wakeup() {
	  this.notifyAll();
	}
    }
}
	      
	  
