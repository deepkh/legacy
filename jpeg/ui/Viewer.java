package ui;
import jpeg.*;
import img.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.io.*;
import java.awt.image.*;

public class Viewer extends JFrame implements Log {
    JMenuBar bar = new JMenuBar();
    OpenFile open = new OpenFile();
    SaveAs saveas = new SaveAs();

    About about = new About();
    Exit exit = new Exit();
    JMenu op = new JMenu("檔案");
    JMenu other = new JMenu("說明");
    JLabel show = new JLabel();
    JPanel mpane = new JPanel(new GridBagLayout());
    JLabel status = new JLabel();
    JFileChooser fc = new JFileChooser();
GridBagConstraints g;



    class OpenFile extends AbstractAction {
	public OpenFile() {
	    super("Load File (*.bmp *.raw)");
	}
	public void actionPerformed(ActionEvent e) {
	    if (fc.showOpenDialog(Viewer.this) 
		== JFileChooser.APPROVE_OPTION)
	    openFile(fc.getSelectedFile().toString());
	}
    }

    class SaveAs extends AbstractAction {
	public SaveAs() {
	    super("Save as JPEG File");
	}
	public void actionPerformed(ActionEvent e) {
	    if (fc.showSaveDialog(Viewer.this) 
		== JFileChooser.APPROVE_OPTION) {
		String Quality = null;
		if ((Quality = JOptionPane.showInputDialog("輸入壓縮率 1 ~ 100。[1 為最差，100 最好]")) != null)
		    saveDCTFile(fc.getSelectedFile().toString(), Integer.parseInt(Quality));
	    }
	}
    }

    class Exit extends AbstractAction {
	public Exit() {
	    super("離開");
	}

	public void actionPerformed(ActionEvent e) {
	    System.exit(1);
	}
    }

    class About extends AbstractAction {
	public About() {
	    super("關於");
	}
	public void actionPerformed(ActionEvent e) {
	    System.exit(1);
	}
    }

    BufferedImage img;
    String openFile;

    void saveDCTFile(String outFile, int Quality) {
	try {
	    if (outFile == null) return;
	    if (outFile.equals("")) return;
	    Encoder.encode(openFile, outFile+"_"+Quality+".raw", Quality, true, this);
	} catch (Exception e) {
	    e.printStackTrace();
	}
    }

    void openFile(String fileName) {
	try {
	    if (fileName == null) return;
	    if (fileName.equals("")) return;
	    this.openFile = fileName;
	    int w = 0, h = 0, size = 0;
	    FileInputStream in = new FileInputStream(fileName);
	    size = (int) in.available();
	    long l = System.currentTimeMillis();
	    if (fileName.toLowerCase().lastIndexOf(".raw") != -1)
		img = Decoder.decode(fileName);
	    else if (fileName.toLowerCase().lastIndexOf(".bmp") != -1)
		img = BMP.read(in);
	    else 
		return;
	    in.close();
	    w = img.getWidth(); h = img.getHeight();
	    show.setIcon(new ImageIcon(img));
	    status.setText(w+"x"+h+"    "+size/1000.0f+"KB    "+
		    (System.currentTimeMillis()-l)/1000+"秒");
	    setTitle(fileName+" - JPEG Viewer V1.0 by deepkh@gmail.com 2006-11-27");
	    if (img.getWidth() >= 1024 || img.getHeight() >= 768)
		setSize(1024, 700);
	    else
		setSize(w+20, h+90);
	    setLocationRelativeTo(null);
	    validate();
	} catch (Exception e) {
	    e.printStackTrace();
	}
    }

    public Viewer() {
	this("");
    }

    public void addUI(JPanel p, JComponent comp, int x, int y, int w, int h, double wx, double wy) {
	g.gridx = x; g.gridy = y;
	g.gridwidth = w; g.gridheight = h;
	g.weightx = wx; g.weighty = wy;
	p.add(comp, g);
    }
    
    public Viewer(String fileName) {
	super(fileName+" - Viewer V1.0 by deepkh@gmail.com");

	op.add(new JMenuItem(open));
	op.addSeparator();
	op.add(new JMenuItem(saveas));
	op.addSeparator();
	op.add(new JMenuItem(exit));
	other.add(new JMenuItem(about));
	bar.add(op);
	bar.add(other);
	setJMenuBar(bar);

	g = new GridBagConstraints();
	g.fill = g.BOTH;
	g.insets = new Insets(1, 1, 1, 1);
	g.anchor=GridBagConstraints.CENTER;

	addUI(mpane, show, 0, 0, 1, 1, 1.0, 0.99);
	g.fill = g.NONE;
	addUI(mpane, status, 0, 1, 1, 1, 1.0, 0.01);
	
//	mpane.add(show, BorderLayout.CENTER);
//	mpane.add(status, BorderLayout.SOUTH);
	setContentPane(new JScrollPane(mpane));
	openFile(fileName);
    }

    public static void main(String[] a) {
	Viewer v = null; 
	if (a.length >= 1) {
	    v = new Viewer(a[0]);
	} else {
	    v = new Viewer();
	    v.setSize(256, 256);
	}
	
	v.setLocationRelativeTo(null);
	v.setVisible(true);
	v.setDefaultCloseOperation(v.EXIT_ON_CLOSE);
    }

    public void println(String str) {
    }

    public void print(String str) {
    }
}
