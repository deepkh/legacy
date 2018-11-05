package transget;

public class SeedPoolStatus {
    /** sync key */
    public Object labelKey = new Object();
    /** rows changed update */
    public boolean update;
    public int[] rows = new int[2];
    public boolean fireTable;
    /** Status Bar Label */
    public boolean fileNumLabelChanged;
    public boolean fileSizeLabelChanged;
    public boolean fileSpeedLabelChanged;
    public String fileNumLabel;
    public String fileSizeLabel;
    public String fileSpeedLabel;

    RuntimeThread runtime;
    SeedStatusUpdater status;

    public SeedPoolStatus(RuntimeThread runtime, SeedStatusUpdater status) {
	this.runtime = runtime;
	this.status = status;
    }

    public String getFileNumLabel() {
	fileNumLabelChanged = false;
	return fileNumLabel;
    }

    public String getFileSizeLabel() {
	fileSizeLabelChanged = false;
	return fileSizeLabel;
    }
    
    public String getSpeedLabel() {
	fileSpeedLabelChanged = false;
	return fileSpeedLabel;
    }
    
    public void updateFileNum() {
	synchronized(runtime.getSyncKey()) {
	    int total = runtime.getSize(), completed = 0, errors = 0;
	    for (int i=0; i<total; i++) {
		Seed seed = runtime.get(i);
		int status = seed.getStatus();
		if (status == Seed.FINISH)
		    completed++;
		if (status == Seed.PARTIAL)
		    errors++;
	    }
	    this.fileNumLabel = total+"/"+completed+"/"+errors;
	    this.fileNumLabelChanged = true;
	    status.updateLabel();
	}
    }
    
    public void updateFileSize() {
	synchronized(runtime.getSyncKey()) {
	    int total = runtime.getSize();
	    long all = 0, received = 0;
	    for (int i=0; i<runtime.getSize(); i++) {
		Seed seed = runtime.get(i);
		int status = seed.getStatus();
		if (status == Seed.FINISH) {
		    total += seed.getFileStatus().contentSize;
		    received += seed.getFileStatus().fileSize;
		}
	    }
	    this.fileSizeLabel = total+"/"+received;
	    this.fileNumLabelChanged = true;
	    status.updateLabel();
	}
    }

    public void updateSpeed(String speed) {
	this.fileSpeedLabel = speed;
	this.fileSpeedLabelChanged = true;
	status.updateLabel();
    }

    public synchronized void fireTable() {
	this.fireTable = true;
	this.update = false;
	updateFileNum();
	updateFileSize();
	status.update();
    }

    public synchronized void getFireTable() {
	this.fireTable = false;
    }

    public synchronized int[] getUpdateRange() {
	this.update = false;
	return rows;
    }

    public synchronized void updateTableRange(int[] r) {
	synchronized(runtime.getSyncKey()) {
	    int min = 0, max = 0;
	    min = r[0];
	    max = r[r.length-1];
	    /*
	    for (int i=0; i<rows.length; i++) {
		Seed seed = runtime.get(rows[i]);
		if (i == 0)
		    min = rows[i];
		else
		    max = rows[i];
	    }*/
	    if (min > max) {
		int tmp = min;
		min = max;
		max = tmp;
	    }
	    rows[0] = min;
	    rows[1] = max;

	    this.fireTable = false;
	    this.update = true;
	    status.update();
	}
    }

    public synchronized void updateTableRange(int id) {
	this.fireTable = false;
	this.update = true;
        status.update();
    }
}
	
