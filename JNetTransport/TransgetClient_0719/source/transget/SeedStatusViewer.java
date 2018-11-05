package transget;

public interface SeedStatusViewer {
  /** update the seed status */
  public void updateTableRange(int[] rows);
  public void updateTableRange(int rows);
  public void updateAllTable();
  public void updateRunningSeed(String speed);
  public void fireTable();
}
