package transget;

public abstract class SeedURL {
  
  protected String url;
  protected String path;
  protected String fileName;
  protected String host;
  protected int port;

  public static void main(String[] arg) {
    String a = "httP://ziwen.no-ip.biz/dir/file.z";
    SeedURL.test(a);
    String b = "httP://ziwen.no-ip.biz:8080/dir/file.z";
    SeedURL.test(b);
    String c = "ftP://ziwen.no-ip.biz/dir/file.z";
    SeedURL.test(c);
    String d = "ftP://ziwen.no-ip.biz:2121/dir/file.z";
    SeedURL.test(d);
    String e = "ftP://anonymous@ziwen.no-ip.biz/dir/file.z";
    SeedURL.test(e);
    String f = "ftP://anonymous:abcdz@ziwen.no-ip.biz:2131/dir/file.z";
    SeedURL.test(f);
  }


  static void test(String u) {
    SeedURL s = SeedURL.newInstance(u);
    System.out.println("\nurl "+s.getURL()+" - - - - - - - -");
    System.out.println("host:"+s.getHost());
    System.out.println("port:"+s.getPort());
    System.out.println("file:"+s.getFileName());
    System.out.println("path:"+s.getPath());
    if (s instanceof SeedFtpURL ) {
      System.out.println("user:"+((SeedFtpURL)s).getUserName());
      System.out.println("pass:"+((SeedFtpURL)s).getPasswd());
    } else {
      System.out.println("referer:"+((SeedHttpURL)s).getReferer());
    }
  }

  public String getURL() {
    return this.url;
  }

  public void setURL(String u) {
    url = u;
    update();
  }

  public String getPath() {
    return this.path;
  }

  public String getFileName() {
    return fileName;
  }

  public String getHost() {
    return host;
  }
  
  public int getPort() {
    return port;
  }
  
  
  public final static SeedURL newInstance(String u) {
    if (u.toLowerCase().indexOf("http") != -1 ) {
      return new SeedHttpURL(u);
    } /*else if (s.toLowerCase().indexOf("ftp") != -1) {
      return new SeedFtpURL(s);
    }*/
    return null;
  }
  
  public abstract String getTransportClass();
  abstract void update();
}
