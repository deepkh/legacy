package ui;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.border.*;
import java.io.*;
import jpeg.*;

public class JPEGEmulation extends JFrame implements Log {

    public static void main(String[] a) {
	new JPEGEmulation();
    }

    GridBagConstraints g;
    JCheckBox ratio[] = new JCheckBox[] {
	new JCheckBox("10%"),
	new JCheckBox("20%"),
	new JCheckBox("30%"),
	new JCheckBox("40%"),
	new JCheckBox("50%"),
	new JCheckBox("60%"),
	new JCheckBox("70%"),
	new JCheckBox("80%"),
	new JCheckBox("90%"),
	new JCheckBox("100%"),
    };

    JTextArea msgs = new JTextArea();
    JButton compress;

    public JPEGEmulation() {
	super("JPEG Encoder V1.0 by deepkh@gmail.com 2006/11/27");
	compress = new JButton(new OpenFile());
	JPanel ratiop = new JPanel(new FlowLayout(FlowLayout.LEFT));
	ratiop.setBorder(	    new TitledBorder(
		new EtchedBorder(EtchedBorder.LOWERED )
		, "Compression Ratio"
	    )
	);
	for (int i=0; i<ratio.length; i++)
	    ratiop.add(ratio[i]);

	msgs.setEditable(false);
	msgs.setBackground(getBackground());
	JScrollPane js = new JScrollPane(msgs,
		JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,
		JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS);

	js.setBorder(new TitledBorder(
		new EtchedBorder(EtchedBorder.LOWERED)
		, "Log"
	    )
	);

	JPanel action = new JPanel(new FlowLayout());
	action.setBorder(
		new EtchedBorder(EtchedBorder.LOWERED)
	);
	action.add(compress);


	g = new GridBagConstraints();
	g.fill = g.BOTH;
	g.insets = new Insets(1, 1, 1, 1);
	g.anchor=GridBagConstraints.CENTER;
	JPanel p = new JPanel(new GridBagLayout());
	addUI(p, ratiop, 0, 0, 1, 1, 1.0, 0.01);
	addUI(p, js, 0, 3, 1, 1, 1.0, 0.98);
	addUI(p, action, 0, 4, 1, 1, 1.0, 0.01);

	setContentPane(p);
	setSize(580, 480);
	setLocationRelativeTo(null);
	setVisible(true);
	setDefaultCloseOperation(EXIT_ON_CLOSE);
    }

    JFileChooser fc = new JFileChooser();
    class OpenFile extends AbstractAction {
	public OpenFile() {
	    super("JPEG Encoder");
	}
	public void actionPerformed(ActionEvent e) {
	    new File("out").mkdirs();
	    if (fc.showOpenDialog(JPEGEmulation.this) 
		== JFileChooser.APPROVE_OPTION)
	    new Thread(new Runnable() {
		public void run() {
		  try {
		    compress.setEnabled(false);
		    for (int j=0; j<ratio.length; j++) {
			if (ratio[j].isSelected()) {
			    int Q = (j+1) * 10;
			    String name = fc.getSelectedFile().getName();
			    jpeg.Encoder.encode(
				fc.getSelectedFile().toString(), 
				"out/"+name+"_"+Q+".raw",
				Q, true, JPEGEmulation.this);
			}
		    }
		    compress.setEnabled(true);
		  } catch (Exception e) {
		      e.printStackTrace();
		      println(e.toString());
		  }
		}
	    }).start();
	}
    }

    public void addUI(JPanel p, JComponent comp, int x, int y, int w, int h, double wx, double wy) {
	g.gridx = x; g.gridy = y;
	g.gridwidth = w; g.gridheight = h;
	g.weightx = wx; g.weighty = wy;
	p.add(comp, g);
    }

    public void println(String str) {
	msgs.append(str+"\r\n");
	msgs.setCaretPosition(msgs.getDocument().getLength()); 
    }

    public void print(String str) {
	msgs.append(str);
	msgs.setCaretPosition(msgs.getDocument().getLength()); 
    }
}
