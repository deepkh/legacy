package transget;
import java.io.File;
import java.io.IOException;
import java.io.OutputStream;
import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.net.Socket;
import java.net.InetSocketAddress;
import java.net.SocketTimeoutException;
import java.util.StringTokenizer;

public class SeedHttpTransport extends SeedTransport 
				implements Runnable {
    /**
     *	    Http Stream Exception
     */
    class HttpTransportException extends Exception {;}
    class HttpLengthException extends HttpTransportException {;}
    class HttpRedirectException extends HttpTransportException {;}
    class HttpStatusException extends HttpTransportException {;}
    class HttpStatus411Exception extends HttpTransportException {;}
    class HttpStopedException extends HttpTransportException {;}

    /**
     *	    Exception Status Code
     */
    final static byte TIM = 0x00;
    final static byte LEN = 0x01;
    final static byte RED = 0x02;
    final static byte STA = 0x03;
    final static byte INT = 0x04;
    final static byte F11 = 0x05;
    final static byte IOE = 0x06;
    final static byte FSH = 0x07;
    final static byte CANCEL = 0x08;

    final static byte[] msgType = {4,4,3,4,4,4,4,3,4};
    final static String[] msg = {
    "connecting ",
    "time out of delay time. re-connection.",
    "remote length not same as local file.",
    "remote file redireced. re-connection.",
    "unknow remote status code.",
    "remote machine interrupted. re-connection.",
    "status code is 411.may be giga mirror. re-connection.",
    "IOException retry.",
    "finished.",
    "user canceled."};

    protected Thread thread;
         
    public synchronized void start() {
      seed.setStatus(Seed.RUN);
      seed.getFileStatus().init(false);
      ((SeedHttpURL)seed.getURL()).setRange(seed.getFileStatus().fileSize);
      thread = new Thread(this);
      thread.start();
    }

    public synchronized void stop() {
      if (thread != null) {
	thread.stop();
      }
    }

    public void retry() {
      if (seed.getTryed() == seed.getMaxtry()) {
	logger("try time to much. close it.",4);
	partial();
	return;
      }
      seed.setTryed(seed.getTryed()+1);
      start();
    }

    public void noContentSizeRetry() {
      if (seed.noContentSizeTryed > 0) {
	seed.noContentSizeTryed--;
	start();
      } else {
	partial();
      }
    }
    
    public void finish() {
      if (seed.getStatus() == Seed.STOP) return;
      seed.setStatus(seed.FINISH);
      seed.getFileStatus().init(true);
      setChanged();
      notifyObservers();
    }

    public void partial() {
      seed.setStatus(seed.PARTIAL);
      seed.getFileStatus().init(true);
      setChanged();
      notifyObservers();
    }

    public void error() {
      partial();
    }

    public void cancel() {
      seed.setStatus(Seed.STOP);
      seed.getFileStatus().init(false);
      thread = null;
      setChanged();
      notifyObservers();
      /*
      if (object != null) {
	synchronized(object) {
	    object.notifyAll();
	}
      }*/
      /*setChanged();
      notifyObservers();*/
    }


    public void run() {
      Socket socket = null;
      DataInputStream in = null;    DataOutputStream out = null;
      FileOutputStream fos = null;  BufferedStream fbuffer = null;
      int status = CANCEL, statcode = 0;
      boolean range = false;
      try {
	    logger(msg[0]+" "+seed.getURL().getHost(),3);
	    socket = new Socket();
	    socket.connect(
	    new InetSocketAddress(
		    seed.getURL().getHost(),seed.getURL().getPort()),
		    seed.getDelay());
	    in = new  DataInputStream(socket.getInputStream());
	    out = new DataOutputStream(socket.getOutputStream());
	    /** put */
	    String head = ((SeedHttpURL)seed.getURL()).getHeadString();
	    multiLog(head, 1);
	    String str; 
	    writeLine(head, out);
	    SeedFileStatus fileStatus = seed.getFileStatus();
	    while (!(str = readLine(in).toLowerCase()).equals("")) {
		if ( str.indexOf("http/1.1") != -1 || 
		     str.indexOf("http/1.0") != -1) {	
		    statcode = Integer.parseInt(str.substring(
		    str.indexOf(" ")+1,str.indexOf(" ")+4));
		}
		if (str.indexOf("content-range") != -1) {
		    fileStatus.contentSize = Long.parseLong(str.substring(
					str.lastIndexOf("/")+1,str.length()));
		    range = true;
		}
		if (str.indexOf("content-length") != -1) {
		    if (range) continue;
		    fileStatus.contentSize = Long.parseLong(
				str.substring(str.indexOf(" ")+1,str.length()));
		}
		if (str.indexOf("location") != -1) {
		    String s = str.substring(str.indexOf(" ")+1,str.length());
		    seed.getURL().setURL(s);
		    //seed.url = new HttpURL(s);
		}
		/** get */
		logger(str, 2);
	    }

	    if (fileStatus.fileSize > 0 && 
			    (fileStatus.contentSize == fileStatus.fileSize)) {
	      status = FSH;
	      stop();
	    }
	    
	    if (statcode >= 300 && statcode < 400)
	    throw new HttpRedirectException();
	    
	    if (statcode == 411)
	    throw new HttpStatus411Exception();
	    
	    if (statcode<200 || statcode>=400)
	    throw new HttpStatusException();

	    if (fileStatus.fileSize > 0)
	    if (fileStatus.fileSize > fileStatus.contentSize)
	    throw new HttpLengthException();
	    
	    if (fileStatus.fileSize > 0) {
	      fos = new FileOutputStream(
			    new File(fileStatus.saveDir,fileStatus.fileName)
			    ,true);
	    } else {
	      fos = new FileOutputStream(
			    new File(fileStatus.saveDir,fileStatus.fileName));
	    }

	    System.out.println("reads: "+fileStatus.reads+
				" ;content-size: "+fileStatus.contentSize);
	    fbuffer = new BufferedStream(fos,seed.getBuffer());
	    logger("receiving data.",3);
	    int reads = 0;  byte[] buffer = new byte[30720];
	    while((reads = in.read(buffer)) != -1) {
	      fbuffer.write(buffer,0,reads);
	      fileStatus.reads += reads;
	      if (fileStatus.reads >= fileStatus.contentSize) break;
	    }
	    
	    if (fileStatus.reads != fileStatus.contentSize)
	    throw new HttpStopedException();

	    status = FSH;
	}
	catch (SocketTimeoutException e3)   {status = TIM;}
	catch (HttpLengthException e4)	    {status = LEN;}
	catch (HttpRedirectException e5)    {status = RED;}
	catch (HttpStatusException e6)	    {status = STA;}
	catch (HttpStatus411Exception e7)   {status = F11;}
	catch (HttpStopedException e8)	    {status = INT;}
	catch (IOException e9)		    {status = IOE;e9.printStackTrace();}
	finally {
	  try {
	    fbuffer.flush(); fos.flush();
	    fbuffer.close(); fos.close();
	    fbuffer = null;  fos = null;
	    in.close(); out.close(); socket.close();
	    in=null; out=null; socket=null;
	  } catch (Exception e) {;}
	    logger(msg[status+1], msgType[status]);
	  if (status == TIM) {
	    if (seed.getFileStatus().reads == 0) {
	      noContentSizeRetry();
	    } else {
	      start();
	    }
	  } else if (status==TIM || status==LEN || status==RED
		|| status==F11 || status==INT) {
	    start();
	  } else if (status==IOE) {
	    retry();
	  } else if (status==STA) {
	    error();
	  } else if (status==FSH) {
	    finish();
	  } else if (status==CANCEL) {
	    cancel();
	  }
	}
    }

    private final void multiLog(String log, int loglev) {
      StringTokenizer stk = new StringTokenizer(log, "\n");
      while(stk.hasMoreTokens()) {
	logger(stk.nextToken(), loglev);
      }
    }

    private final void logger(String log, int loglev) {
      if (loglev == 1 )
	seed.getLogger().logSeedPutMsg(log);
      else if (loglev == 2)
	seed.getLogger().logSeedGetMsg(log);
      else if (loglev == 3)
	seed.getLogger().logSeedWarningMsg(log);
      else
	seed.getLogger().logSeedErrorMsg(log);
    }	

    final static byte[] CRLF = "\r\n".getBytes();
    
    private final void writeLine(String Str,DataOutputStream out)
    throws IOException {
    	out.write(Str.getBytes());
	out.write(CRLF);
	out.flush();
    }
    
    private final String readLine(DataInputStream in)
    throws IOException {
    	StringBuffer sb = new StringBuffer("");
	int len = 0;
	byte[] b = new byte[1];
        byte before = 0;
        while(true) {
            b[0] = in.readByte();
            if (b[0] == 13)
                before = (byte) 13;
            if (b[0] == 10) {
                if (before == 13) {
                    String str = sb.toString();
                    return str.substring(0, str.length()-1);
                } else {
		    String str = sb.toString();
                    return sb.toString();
                }
            }
            sb.append(new String(b));
            len++;
        }
    }

    class BufferedStream extends BufferedOutputStream {
	public BufferedStream(OutputStream out,int buffer) {
	    super(out);
	    super.buf = new byte[buffer];
	}
    }
}
