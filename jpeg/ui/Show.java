package ui;
import jpeg.*;
import img.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.io.*;
import java.awt.image.*;

public class Show extends JFrame {
GridBagConstraints g;
    static int count;
    static int count2;

    public void addUI(JPanel p, JComponent comp, int x, int y, int w, int h, double wx, double wy) {
	g.gridx = x; g.gridy = y;
	g.gridwidth = w; g.gridheight = h;
	g.weightx = wx; g.weighty = wy;
	p.add(comp, g);
    }

    public Show(String title, BufferedImage comp) {
	super("JPEG Decoder - "+title.split(" ")[0]+" "+title.split(" ")[1]+" "+title.split(" ")[2]);
	JPanel p = new JPanel(new GridBagLayout());
	JPanel p2 = new JPanel(new GridBagLayout());
	JPanel mp = new JPanel(new GridBagLayout());
	g = new GridBagConstraints();
	g.fill = g.BOTH;
	g.insets = new Insets(1, 1, 1, 1);
	g.anchor=GridBagConstraints.CENTER;

	JLabel cl = new JLabel();
	cl.setIcon(new ImageIcon(comp));

	addUI(p, cl, 0, 0, 1, 1, 1.0, 1.0);
	
	g = new GridBagConstraints();
	g.fill = g.BOTH;
	g.insets = new Insets(1, 1, 1, 1);
	g.anchor=GridBagConstraints.CENTER;

	for (int i=0; i<title.split(" ").length; i++)
	    addUI(p2, new JLabel(title.split(" ")[i]), 0, i, 1, 1, 1.0, 1.0);
	
	
	g = new GridBagConstraints();
	g.fill = g.BOTH;
	g.insets = new Insets(1, 1, 1, 1);
	g.anchor=GridBagConstraints.CENTER;

	addUI(mp, p, 0, 0, 1, 1, 1.0, 1.0);
	addUI(mp, p2, 0, 1, 1, 1, 1.0, 0.1);
	if (comp.getWidth() <= 1024 && comp.getHeight() <= 768) {
	    setContentPane(mp);   
	    setSize(comp.getWidth(), comp.getHeight()+170);
	    int z = 1024 / comp.getWidth();
	    int z2 = 768 / comp.getHeight();
	    int x = count * comp.getWidth();
	    int y = count2 * (comp.getHeight()+85);
	    count = (++count) % z;
	    if (count == 0)
		count2 = (++count2) % z2;
	    setLocation(x, y);
	} else {
	    setContentPane(new JScrollPane(mp));	    
	    setSize(1024, 700);
	}
	setVisible(true);
    }
}
