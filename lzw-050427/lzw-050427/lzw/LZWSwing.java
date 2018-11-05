/*
 * 在 2005/4/24 建立
 *
 * TODO 如果要變更這個產生的檔案的範本，請移至
 * 視窗 - 喜好設定 - Java - 程式碼樣式 - 程式碼範本
 */
package lzw;
import java.awt.Color;
import java.awt.Font;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;

import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JProgressBar;
import javax.swing.SwingUtilities;
import javax.swing.border.BevelBorder;
import javax.swing.border.LineBorder;
import javax.swing.border.SoftBevelBorder;
import javax.swing.border.TitledBorder;
/**
 * @author Administrator
 *
 * TODO 如果要變更這個產生的類別註解的範本，請移至
 * 視窗 - 喜好設定 - Java - 程式碼樣式 - 程式碼範本
 */
interface LZWMonitor {
	public void updateProgress(int reading, int total);
}
public class LZWSwing extends JPanel implements LZWMonitor {

	JProgressBar compress;
	JButton btCompress, btDecompress;
	
	public static void main(String[] args) {
		JFrame f = new JFrame("The LZW Compress/Decompress ");
		JPanel jpa = new LZWSwing();
		GridBagConstraints g = new GridBagConstraints();
		g.fill = g.BOTH;
		g.gridx = 0;
		g.gridy = 0;
		g.gridwidth = 1;
		g.gridheight = 1;
		g.weightx = 1.0;
		g.weighty = 1.0;
		g.insets = new Insets(10,10,10,10);
		JPanel jpb = new JPanel(new GridBagLayout());
		jpb.add(jpa, g);
		f.setContentPane(jpb);
		f.setSize(400,180);
		f.setLocationRelativeTo(null);
		f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		f.setVisible(true);
	}
	
	public LZWSwing() {
		setLayout(new GridBagLayout());
		compress = new JProgressBar();
		compress.setStringPainted(true);
		btCompress = new JButton("Compress");
		btDecompress = new JButton("Decompress");
		btCompress.setBorder(LineBorder.createGrayLineBorder());
		btDecompress.setBorder(LineBorder.createGrayLineBorder());
		TitledBorder b1 = new TitledBorder(new SoftBevelBorder(BevelBorder.LOWERED),
				"The LZW Compress/Decompress", TitledBorder.RIGHT, TitledBorder.BOTTOM);
		b1.setTitleColor(new Color(99,132,165));
		b1.setTitleFont(new Font("Arial",Font.ITALIC,12));
		
		setBorder(b1);

		GridBagConstraints g = new GridBagConstraints();
		g.insets = new Insets(5,5,5,5);
		g.fill = g.BOTH;
		g.gridx = 0;
		g.gridy = 0;
		g.gridwidth = 2;
		g.gridheight = 1;
		g.weightx = 1.0;
		g.weighty = 1.0;
		add(compress, g);
		
		g.fill = g.HORIZONTAL;
		g.insets = new Insets(0,5,5,5);
		g.gridy = 1;
		g.gridwidth = 1;
		add(btCompress,g);
		
		g.gridx = 1;
		add(btDecompress,g);
		//updateProgress(0,0);

		btCompress.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				JFileChooser f = new JFileChooser(current);
				if(f.showOpenDialog(null) == f.APPROVE_OPTION) {
					toCompress(f.getSelectedFile().getAbsolutePath());
					current = new File(f.getSelectedFile().getPath());
					btCompress.setEnabled(false);
					btDecompress.setEnabled(false);
				}
				
			}
		});
		
		btDecompress.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				JFileChooser f = new JFileChooser(current);
				if(f.showOpenDialog(null) == f.APPROVE_OPTION) {
					toDecompress(f.getSelectedFile().getAbsolutePath());
					current = new File(f.getSelectedFile().getPath());
					btCompress.setEnabled(false);
					btDecompress.setEnabled(false);
				}					
			}
		});
		
		//new Thread(ur).start();
	}
	

	int reading, total;
	public void updateProgress(int reading, int total) {
		compress.setValue(this.reading = reading);
		compress.setMaximum(this.total = total);
		if (reading != 0 || total != 0)
			compress.setString(reading+"/"+total);
	}

	
	public void showMessage(boolean success, String msg) {
		JOptionPane.showMessageDialog(null,msg);
		btCompress.setEnabled(true);
		btDecompress.setEnabled(true);
	}
	
	LZWMonitor monitor = this;
	File current = new File(System.getProperty("user.home"));
	void toCompress(final String source) {
		Thread t = new Thread(new Runnable() {
			public void run() {
				try {
					long b = System.currentTimeMillis();
					LZWCompress c = new LZWCompress(source, monitor);
					c.compress();
					showMessage(true, "Compress Successful! cost "+(System.currentTimeMillis()-b)/(double)1000+" secs.");
					updateProgress(total,total);
				} catch (Exception e) {
					showMessage(false, "Compression Fail!");
				}
			}
		});
		t.start();
	}
	
	
	void toDecompress(final String source) {
		Thread t = new Thread(new Runnable() {
			public void run() {
				try {
					long b = System.currentTimeMillis();
					LZWDecompress c = new LZWDecompress(source, monitor);
					c.decompress();
					showMessage(true, "Decompress Successful! cost "+(System.currentTimeMillis()-b)/(double)1000+" secs.");
					updateProgress(total,total);
				} catch (Exception e) {
					showMessage(false, "Decompression Fail!");
				}
			}
		});
		t.start();
	}
}
