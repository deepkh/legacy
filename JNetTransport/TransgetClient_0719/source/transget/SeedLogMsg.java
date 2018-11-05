package transget;
import java.text.DateFormat;
import java.util.Date;

public class SeedLogMsg {

   int type;
  private String date;
  private String msg;

  public SeedLogMsg(int type, String msg) {
    this.type = type;
    this.msg = msg;
    this.date = DateFormat.getInstance().format(new Date());
  }

  public int getType() {
    return type;
  }

  public String getDate() {
    return date;
  }

  public String getMsg() {
    return msg;
  }
}
