package transget.swing;
import java.util.Locale;
import java.util.ResourceBundle;

public class Resources {

  static String baseName = "transget/swing/resources/JNetTransport";

  public static void main(String a[]) {
    p(getLabel("start"));
    p(getLabel("startAll"));
    p(getLabel("stop"));
    p(getLabel("stopAll"));
  }

  static void p(String p) {
    System.out.println(p);
  }

  static ResourceBundle getBundle() {
    ResourceBundle resource = null;
    try {
	resource = ResourceBundle.getBundle(baseName,Locale.getDefault());
      } catch (Exception e) {
	e.printStackTrace();
	resource = ResourceBundle.getBundle(baseName);
      }
    return resource;
  }	

  public static String getLabel(String key) {
    String str = getBundle().getString(key);
    return str;
  }
}
