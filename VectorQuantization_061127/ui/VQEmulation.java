package ui;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.border.*;
import java.io.*;
import vq.*;

public class VQEmulation extends JFrame implements Log {

    public static void main(String[] a) {
	new VQEmulation();
    }

    int val[] = new int[] {
	2, 4, 8, 16, 32, 64, 128, 256, 512
    };

    GridBagConstraints g;
    JCheckBox blockBox[] = new JCheckBox[] {
	new JCheckBox("2x2"),
	new JCheckBox("4x4"),
	new JCheckBox("8x8"),
	new JCheckBox("16x16")
    };

    JCheckBox codeBookBox[] = new JCheckBox[] {
	new JCheckBox("2"),
	new JCheckBox("4"),
	new JCheckBox("8"),
	new JCheckBox("16"),
	new JCheckBox("32"),
	new JCheckBox("64"),
	new JCheckBox("128"),
	new JCheckBox("256"),
	new JCheckBox("512")
    };

    JTextField threshold = new JTextField(5);
    JTextArea msgs = new JTextArea();
    JButton compress;
    JButton about = new JButton("關於");

    public VQEmulation() {
	super("VQ Emulation V1.0 by deepkh@gmail.com 2006/10/27");
	threshold.setText("0.1");
	compress = new JButton(new OpenFile());
	JPanel blocks = new JPanel(new FlowLayout(FlowLayout.LEFT));
	blocks.setBorder(	    new TitledBorder(
		new EtchedBorder(EtchedBorder.LOWERED )
		, "Block Size"
	    )
	);
	for (int i=0; i<blockBox.length; i++)
	    blocks.add(blockBox[i]);
	JPanel codeBook = new JPanel(new FlowLayout(FlowLayout.LEFT));
	codeBook.setBorder(	    new TitledBorder(
		new EtchedBorder(EtchedBorder.LOWERED )
		, "Code Book"
	    )
	);
	for (int i=0; i<codeBookBox.length; i++)
	    codeBook.add(codeBookBox[i]);
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
	//action.add(about);

	JPanel thp = new JPanel(new FlowLayout(FlowLayout.LEFT));
	thp.add(threshold);
	thp.setBorder(new TitledBorder(
		new EtchedBorder(EtchedBorder.LOWERED)
		, "Threshold"
	    )
	);

	g = new GridBagConstraints();
	g.fill = g.BOTH;
	g.insets = new Insets(1, 1, 1, 1);
	g.anchor=GridBagConstraints.CENTER;
	JPanel p = new JPanel(new GridBagLayout());
	addUI(p, blocks, 0, 0, 1, 1, 1.0, 0.01);
	addUI(p, codeBook, 0, 1, 1, 1, 1.0, 0.01);
	g.fill = g.NONE;
	g.anchor = g.WEST;
	addUI(p, thp, 0, 2, 1, 1, 1.0, 0.01);
	g.fill = g.BOTH;
	addUI(p, js, 0, 3, 1, 1, 1.0, 0.97);
	addUI(p, action, 0, 4, 1, 1, 1.0, 0.01);

	setContentPane(p);
	setSize(480, 480);
	setLocationRelativeTo(null);
	setVisible(true);
	setDefaultCloseOperation(EXIT_ON_CLOSE);
    }

    JFileChooser fc = new JFileChooser();
    class OpenFile extends AbstractAction {
	public OpenFile() {
	    super("VQ 壓縮結果");
	}
	public void actionPerformed(ActionEvent e) {
	    if (fc.showOpenDialog(VQEmulation.this) 
		== JFileChooser.APPROVE_OPTION)
	    new Thread(new Runnable() {
		public void run() {
		  try {
		    compress.setEnabled(false);
		    for (int i=0; i<blockBox.length; i++)
		    for (int j=0; j<codeBookBox.length; j++) {
			if (blockBox[i].isSelected() && codeBookBox[j].isSelected()) {
			    new vq.SplitBlock(fc.getSelectedFile().toString(), 
				val[i], val[i], val[j], Double.parseDouble(threshold.getText()), VQEmulation.this);
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
