package transget;

public class SeedFtpURL extends SeedURL {
  
  protected String user = "anonymous";
  protected String pass = "anonymous@yahoo.com.tw";
  protected String className = "transget.SeedFtpTransport";

  public SeedFtpURL(String u) {
    url = u;
    update();
  }

  public String getUserName() {
    return user;
  }

  public void setUserName(String u) {
    user = u;
  }

  public String getPasswd() {
    return pass;
  }

  public void setPasswd(String p) {
    pass = p;
  }
  public void update() {
    String str = url.substring(6, url.length());
    if (str.indexOf("@") != -1) {
      if (str.substring(0,str.indexOf("@")).indexOf(":") != -1) {
	this.user = str.substring(0, str.indexOf(":"));
	this.pass = str.substring(str.indexOf(":")+1, str.indexOf("@"));
      } else {
	this.user = str.substring(0, str.indexOf("@"));
      }
      str = str.substring(str.indexOf("@")+1, str.length());
    }
    super.host = str.substring(0, str.indexOf("/"));
    if (str.indexOf(":") != -1) {
      super.port = Integer.parseInt(
			str.substring(
				str.indexOf(":")+1, str.indexOf("/")));
      super.host = str.substring(0, str.indexOf(":"));
    } else {
      super.port = 21;
    }
    super.fileName = str.substring(str.lastIndexOf("/")+1, str.length());
    super.path = str.substring(str.indexOf("/"), str.length());
  }

  public String getTransportClass() {
    return className;
  }
}
