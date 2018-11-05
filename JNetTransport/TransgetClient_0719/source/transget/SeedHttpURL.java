package transget;

public class SeedHttpURL extends SeedURL {
  
  protected long range;
  protected String referer;
  protected String className = "transget.SeedHttpTransport";

  public SeedHttpURL(String u) {
    url = u;
    update();
  }

  public String getHeadString() {
    StringBuffer sb = new StringBuffer();
    /** GET XXx HTTP/1.1 */
    sb.append("GET ");
    sb.append(getPath());
    sb.append(" HTTP/1.1\r\n");
    
    /** Host */
    sb.append("Host: ");
    sb.append(getHost());
    sb.append("\r\n");
      
    /** Referer */
    sb.append("Referer: ");
    sb.append(getReferer());
    sb.append("\r\n");
      
    /** Range: bytes=123321- */
    if (range > 0) {
      sb.append("Range: bytes=");
      sb.append(range);
      sb.append("-\r\n");
    }
      
    /** Accept: @/@ */
    sb.append("Accept: ");
    sb.append("\r\n");
      
    /** User-Agent: Mozilla */
    sb.append("User-Agent: Mozilla/4.0 (compatible; MSIE 5.00; Windows 98)");
    sb.append("\r\n");
      
    /** Connection: Keep-Alive */
    sb.append("Connection: Keep-Alive");
    sb.append("\r\n");
   
    return sb.toString();
  }

  public String getReferer() {
    return referer;
  }

  public void setReferer(String s) {
    referer = s;
  }

  public void setRange(long size) {
    System.out.println("set range:"+size);
    range = size;
  }

  public String getTransportClass() {
    return className;
  }

  public void update() {
    String str = url.substring(7, url.length());
    super.host = str.substring(0, str.indexOf("/"));
    if ( str.indexOf(":") != -1) {
      super.port = Integer.parseInt(
			str.substring(
				str.indexOf(":")+1, str.indexOf("/")));
      super.host = str.substring( 0, str.indexOf(":"));
    } else {
      super.port = 80;
    }
    if (fileName == null )
      super.fileName = str.substring(str.lastIndexOf("/")+1, str.length());
    super.path = str.substring(str.indexOf("/"), str.length());
    if (referer == null) {
      referer = url.substring(0,url.lastIndexOf("/"));
    }
  }
}
