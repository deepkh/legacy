package transget;

public interface SeedEventModel {

    public void removeFinish();
    
    public void removeSeedRange(int[] rows);

    public void removeAllSeed();

    public void startSeedRange(int[] rows);

    public void startAllSeed();

    public void stopSeedRange(int[] rows);

    public void stopAllSeed();

    public void removeSeedFile(int[] rows);
    
    public void removeErrorsFile();

    public void setSeedZeroRange(int[] rows);
    
    public void setSeedZeroAll();

    public void setErrorSeedZero();
    
    public void updateSeed(int index,String dir,String referer, 
			int delay,int maxtry,int status,
			String fileName,int buffer);

    public void updateSeedSaveDir(int index, String newdir);

    public void updateSeedReferer(int index, String referer);

    public void updateSeedDelay(int index, int delay);

    public void updateSeedMaxtry(int index, int maxtry);

}
