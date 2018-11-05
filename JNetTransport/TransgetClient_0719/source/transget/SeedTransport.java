package transget;
import java.util.Observer;
import java.util.Observable;

public abstract class SeedTransport extends Observable {

  protected Seed seed;

  public void init(Seed s) {
    seed = s;
  }

  public Seed getSeed() {
    return seed;
  }

  public abstract void start();

  public abstract void stop();

  public abstract void finish();

  public abstract void partial();

  public abstract void error();

}
