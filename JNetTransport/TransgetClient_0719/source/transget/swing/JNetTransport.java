package transget.swing; 
import transget.*;
import java.awt.datatransfer.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.table.*;
import javax.swing.border.*;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.Serializable;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.Vector;
import java.util.ArrayList;

public class JNetTransport extends JFrame implements SeedStatusUpdater {
    /**
     *	GUI Variables
     */
    final static Color WHITE = new Color(255,255,255);
    final static Color GREEN = new Color(0,137,27);
    final static Color BLUE = new Color(47,78,149);
    final static Color YELLOW = new Color(146,136,22);
    final static Color RED = new Color(220,34,36);
    final static Color bGREEN = new Color(194,224,204);
    final static Color bBLUE = new Color(205,220,255);
    final static Color bYELLOW = new Color(224,224,193);
    final static Color bRED = new Color(225,193,194);
    final static String defaultFont = "PMingliu";
    final static Font smallFont = new Font(defaultFont, Font.PLAIN, 12);
    final static Font middleFont = new Font(defaultFont, Font.PLAIN, 13);
    final static Font bigFont = new Font(defaultFont, Font.PLAIN, 16);

    protected JNetTransportProperties prop;
    protected RuntimeThread runtime;
    protected SeedEventHandler handler;
   
    /** invoke action */
    protected LookNFeel lnfAct;
    protected About about;
    protected AddSingleDownload singleAct;
    protected AddMultiDownload multiAct;
    protected SetOption optionAct;
    protected ModifiesDownload modifiesAct;
    protected Close closeAct;
    protected StartRange startRangeAct;
    protected StartAll startAllAct;
    protected StopRange stopRangeAct;
    protected StopAll stopAllAct;
    protected Remove rmAct;
    protected RemoveFinish removeFinish;
    protected RemoveFile rmFileAct;
    protected RemoveSeedAndFile rmFileAndSeedAct;
    
    protected SetToZeroRange setToZero;
    protected SetToZeroAndRemove setToZeroRemove;
    protected SetErrorsSeedToZero setErrToZero;
    protected SetErrorsSeedToZeroAndRemove setErrToZeroRemove;

    protected MenuBar menubar;
    protected ToolBar toolbar;
    protected StatusBar statusbar;
    protected AboutDialog aboutDialog;
    protected PropertiesDialog propDialog;
    protected SingleDownload single;
    protected MultiDownload multi;
    protected SeedTable table;
    protected SeedLogTable logTable;

    protected SeedPoolStatus status;

    private static ArrayList windowList = new ArrayList();

    static {
	    UIManager.installLookAndFeel("Kunststoff","com.incors.plaf.kunststoff.KunststoffLookAndFeel");
	    UIManager.installLookAndFeel("Plastic","com.jgoodies.plaf.plastic.Plastic3DLookAndFeel");
	}

    GridBagConstraints g;

    public JNetTransport() {
      super();
      setTitle(getString("title")+" "+getString("version"));
      setLookAndFeel(1);
      windowList.add(this);
      this.runtime = new RuntimeThread(this);
      this.status = this.runtime.getStatus();
      this.handler = this.runtime.getEventHandler();
      this.prop = new JNetTransportProperties();
      this.runtime.setMaxStream(prop.maxstream);
      initInvokeAction();
      initComponent();
      setSize(prop.frame_width,prop.frame_height);
      setLocation(prop.locationX,prop.locationY);
      prop.readSeedURL(runtime, this.status);
      setVisible(true);
      addWindowListener(new WindowAdapter() {
	public void windowClosing(WindowEvent e) {
	  close();
	}
      });
    }

    void initInvokeAction() {
      lnfAct = new LookNFeel();
      about = new About();
      singleAct = new AddSingleDownload();
      multiAct = new AddMultiDownload();
      optionAct = new SetOption();
      modifiesAct = new ModifiesDownload();
      closeAct = new Close();
      startRangeAct = new StartRange();
      startAllAct = new StartAll();
      stopRangeAct = new StopRange();
      stopAllAct = new StopAll();
      /** only range to do */
      removeFinish = new RemoveFinish();
      rmAct = new Remove();
      rmFileAct = new RemoveFile();
      rmFileAndSeedAct = new RemoveSeedAndFile();
    
      setToZero = new SetToZeroRange();
      setToZeroRemove = new SetToZeroAndRemove();
      setErrToZero = new SetErrorsSeedToZero();
      setErrToZeroRemove = new SetErrorsSeedToZeroAndRemove();
    }
    
    void initComponent() {
      g = new GridBagConstraints();
      menubar = new MenuBar();
      toolbar = new ToolBar();
      statusbar = new StatusBar();
      aboutDialog = new AboutDialog(this);
      propDialog = new PropertiesDialog(this);
      single = new SingleDownload(this);
      multi = new MultiDownload(this);
      table = new SeedTable();
      logTable = new SeedLogTable();
      
      JPanel pane = new JPanel(new BorderLayout(0,0));
      JScrollPane scroll1 = new JScrollPane(table);
      scroll1.getViewport().setBackground(WHITE);
      JScrollPane scroll2 = new JScrollPane(logTable);
      scroll2.getViewport().setBackground(WHITE);
      JSplitPane split = new JSplitPane(JSplitPane.VERTICAL_SPLIT,scroll1,
					scroll2);
      split.setResizeWeight(0.5);
      pane.add(toolbar, BorderLayout.NORTH);
      pane.add(split, BorderLayout.CENTER);
      pane.add(statusbar,BorderLayout.SOUTH);
      setJMenuBar(menubar);
      setContentPane(pane);
    }
    
    void setLookAndFeel(int i) {
      /*UIManager.put("ToolBar.font", middleFont);
      UIManager.put("Menu.font", middleFont);
      UIManager.put("MenuItem.font", middleFont);
      UIManager.put("MenuBar.font", middleFont);
      UIManager.put("RadionButton.font", middleFont);
      UIManager.put("OptionPane.font", middleFont);
      UIManager.put("Panel.font", middleFont);
      UIManager.put("TableHeader.font.font", middleFont);
      UIManager.put("PopupMenu.font", middleFont);
      UIManager.put("Button.font", middleFont);
      UIManager.put("Label.font", middleFont);
      i = 4;*/
      /*
      try {
	if (i == 1) {
	  UIManager.setLookAndFeel(
		"com.sun.java.swing.plaf.windows.WindowsLookAndFeel");
	} else if (i == 2) {
	  UIManager.setLookAndFeel(
		"javax.swing.plaf.metal.MetalLookAndFeel");
	} else if (i == 3) {
	  UIManager.setLookAndFeel(
		"com.sun.java.swing.plaf.gtk.GTKLookAndFeel");
	} else {
	  UIManager.setLookAndFeel(
		"com.incors.plaf.kunststoff.KunststoffLookAndFeel");
	}
      } catch (Exception e) {;}
      */
    }
    
    void addCom(GridBagConstraints g, JPanel p, JComponent com,
		int gridwidth, int gridheight, int x, int y) {
      g.gridwidth = gridwidth;
      g.gridheight = gridheight;
      g.gridx = x;
      g.gridy = y;
      p.add(com, g);
    }

    String getClipboardText() {
      Clipboard clip = Toolkit.getDefaultToolkit().getSystemClipboard();
      Transferable t = clip.getContents(this);	
      String str = null;
      try {
	    str = (String) t.getTransferData(DataFlavor.stringFlavor);
      } catch (Exception e) {;}
      return str;
    }

    void setClipboardText(String str) {
      Clipboard clip = Toolkit.getDefaultToolkit().getSystemClipboard();
      StringSelection s = new StringSelection(str);
      clip.setContents(s,s);
    }

    KeyStroke key(int m1, int m2) {
      return KeyStroke.getKeyStroke(m1,m2);
    }

    String getString(String key) {
      return Resources.getLabel(key).intern();
    }

    ImageIcon getIcon(String key) {
      return new ImageIcon(
	    ClassLoader.getSystemClassLoader()
	    .getResource("img1/"+getString(key+".icon")));
    }

    /** 選單 :: MenuBar */
    class MenuBar extends JMenuBar {
	public MenuBar() {
	  JMenu m = new JMenu(getString("menubar.file"));
	  m.add(startAllAct);
	  m.add(stopAllAct);
	  m.addSeparator();
	  m.add(closeAct);
	  add(m);
	  m = new JMenu(getString("menubar.mission"));
	  m.add(singleAct);
	  m.add(multiAct);
	  m.addSeparator();
	  m.add(optionAct);
	  add(m);
	  m = new JMenu(getString("menubar.theme"));
	  ButtonGroup bgroup = new ButtonGroup();
	  UIManager.LookAndFeelInfo[] f = UIManager.getInstalledLookAndFeels();
	  for (int i=0; i<f.length; i++) {
	    JRadioButtonMenuItem rm = new JRadioButtonMenuItem(f[i].getName());
	    rm.setActionCommand(f[i].getClassName());
	    rm.addActionListener(lnfAct);
	    m.add(rm);
	    bgroup.add(rm);
	  }
	  add(m);
	  m = new JMenu(getString("menubar.other"));
	  m.add(about);
	  add(m);
	}

    }

    /** 工具列 :: Toolbar */
    class ToolBar extends JToolBar {
	public ToolBar() {
	  setLayout(new GridLayout(1,7));
	  add(new JButton(startAllAct));
	  add(new JButton(stopAllAct));
	  add(new JButton(singleAct));
	  add(new JButton(rmAct));
	  add(new JButton(multiAct));
	  add(new JButton(optionAct));
	  add(new JButton(closeAct));
	}
    }
    

    /** 速度列 :: SpeedBar */
    class StatusBar extends JPanel {
      JLabel fileNum, fileSize, speed;
      int align = SwingConstants.RIGHT;
      public StatusBar() {
	super(new GridBagLayout());
	
	Border fileb = new TitledBorder(
	    new SoftBevelBorder(SoftBevelBorder.LOWERED),
	    "File (total/completed/errors)",
	    TitledBorder.LEFT, TitledBorder.TOP);
	
	Border sizeb = new TitledBorder(
	    new SoftBevelBorder(SoftBevelBorder.LOWERED),
	    "Size (total/received)",
	    TitledBorder.LEFT, TitledBorder.TOP);
	
	Border speedb = new TitledBorder(
	    new SoftBevelBorder(SoftBevelBorder.LOWERED),
	    "File Speed (KB/s)",
	    TitledBorder.LEFT, TitledBorder.TOP);

	Font font = new Font("PMingLiU",Font.PLAIN, 12);
	
	g.fill = g.BOTH;
	g.gridx = 0;
	g.gridy = 0;
	g.weightx = 0.4;
	fileNum = new JLabel("",align);
	fileNum.setFont(font);
	fileNum.setBorder(fileb);
	add(fileNum, g);

	g.gridx = 1;
	fileSize = new JLabel("",align);
	fileSize.setFont(font);
	fileSize.setBorder(sizeb);
	add(fileSize, g);

	g.gridx = 2;
	g.ipadx = 10;
	g.weightx = 0.2;
	speed = new JLabel("",align);
	speed.setFont(font);
	speed.setBorder(speedb);
	add(speed, g);
      }

      public void setSpeed(String s) {
	speed.setText(s);
      }

      public void setFileNum(String l) {
	fileNum.setText(l);
      }
      
      public void setFileSize(String l) {
	fileSize.setText(l);
      }
    }


    /** 關於對話框 :: About Dialog */
    class AboutDialog extends JDialog {
	  public AboutDialog(JFrame frame) {
	    super(frame, getString("about.title"), true);
	    JTextArea t = new JTextArea();
	    JButton b = new JButton(getString("about.okay"));
	    JPanel p = new JPanel(new BorderLayout());
	    t.setText(getString("about.context"));
	    p.add(t,BorderLayout.CENTER);
	    p.add(b,BorderLayout.SOUTH);
	    setContentPane(p);
	    pack();
	    setLocationRelativeTo(null);
	    b.addActionListener(new ActionListener() {
	      public void actionPerformed(ActionEvent e) {
		close();
	      }
	    });
	    windowList.add(this);
	  }

	  public void open() {
	    this.show();
	  }

	  public void close() {
	    this.hide();
	  }
    }


    /** 屬性設定對話框 :: Properties Dialog */
    class PropertiesDialog extends JDialog implements ActionListener {
	final JTextField tf[] = new JTextField[5];
	final JButton bt[] = new JButton[3];

	public PropertiesDialog(JFrame frame) {
	  super(frame,getString("properties.title"),true);
	  initComponent();
	  setLookAndFeel(1);
	  setSize(500,260);
	  setLocationRelativeTo(null);
	  windowList.add(this);
	}

	public void open() {
	  tf[0].setText(prop.savePath);
	  tf[1].setText(prop.buffer+"");
	  tf[2].setText(prop.maxtry+"");
	  tf[3].setText(prop.delay+"");
	  tf[4].setText(prop.maxstream+"");
	  this.show();
	}

	private void setText() {
	  prop.savePath = tf[0].getText();
	  prop.buffer = Integer.parseInt(tf[1].getText());
	  prop.maxtry = Integer.parseInt(tf[2].getText());
	  prop.delay = Integer.parseInt(tf[3].getText());
	  int maxstream_ = Integer.parseInt(tf[4].getText());
	  prop.maxstream = maxstream_;
	  runtime.setMaxStream(maxstream_);
	}

	void initComponent() {
	  String label[] = {
	    getString("save"), getString("buffer"), getString("retry"),
	    getString("delay"), getString("stream"), getString("browser"),
	    getString("okay"),getString("cancel")};
	  String info[] = {
	    getString("bytes"), getString("times"), getString("millis")};
	  for (int i=0; i<tf.length; i++) tf[i] = new JTextField();
	  for (int i=0;i<bt.length;i++) {
	    bt[i] = new JButton(label[5+i]);
	    bt[i].addActionListener(this);
	  }

	  Border bo = BorderFactory.createEtchedBorder();
	  JPanel main = new JPanel(new GridBagLayout());
	  JPanel field = new JPanel(new GridBagLayout());
	  JPanel button = new JPanel(new GridBagLayout());

	  field.setBorder(new TitledBorder(bo,getString("preferences")));
	  button.setBorder(bo);
	    
	  g.insets = new Insets(2,2,2,2);
	  g.fill = GridBagConstraints.BOTH;
	  g.weighty = 1.0; g.weightx = 1.0;
	    
	  g.weightx = 0.1;
	  addCom(g,field,new JLabel(label[0],SwingConstants.RIGHT),1,1,0,0);
	  g.weightx = 0.8;
	  addCom(g,field,tf[0],1,1,1,0);
	  g.weightx = 0.1;
	  addCom(g,field,bt[0],1,1,2,0);

	  g.weightx = 0.1;
	  addCom(g,field,new JLabel(label[1],SwingConstants.RIGHT),1,1,0,1);
	  g.weightx = 0.8;
	  addCom(g,field,tf[1],1,1,1,1);
	  g.weightx = 0.1;
	  addCom(g,field,new JLabel(info[0],SwingConstants.LEFT),1,1,2,1);

	  g.weightx = 0.1;
	  addCom(g,field,new JLabel(label[2],SwingConstants.RIGHT),1,1,0,2);
	  g.weightx = 0.8;
	  addCom(g,field,tf[2],1,1,1,2);
	  g.weightx = 0.1;
	  addCom(g,field,new JLabel(info[1],SwingConstants.LEFT),1,1,2,2);

	  g.weightx = 0.1;
	  addCom(g,field,new JLabel(label[3],SwingConstants.RIGHT),1,1,0,3);
	  g.weightx = 0.8;
	  addCom(g,field,tf[3],1,1,1,3);
	  g.weightx = 0.1;
	  addCom(g,field,new JLabel(info[2],SwingConstants.LEFT),1,1,2,3);
	    
	  g.weightx = 0.1;
	  addCom(g,field,new JLabel(label[4],SwingConstants.RIGHT),1,1,0,4);
	  g.weightx = 0.8;
          addCom(g,field,tf[4],1,1,1,4);

	  g.weightx = 0.8;
	  addCom(g,button,new JLabel(""),1,1,0,0);
	  g.weightx = 0.1;
	  addCom(g,button,bt[1],1,1,1,0);
	  g.weightx = 0.1;
	  addCom(g,button,bt[2],1,1,2,0);

	  g.weightx = 1.0;
	  g.weighty = 0.9;
	  addCom(g,main,field,1,1,0,0);
	  g.weighty = 0.1;
	  addCom(g,main,button,1,1,0,1);

	  setContentPane(main);
	}

	public void actionPerformed(ActionEvent ae) {
	  JButton b = (JButton) ae.getSource();
	  if (b.getText().equals(getString("okay"))) {
	    setText();
	    hide();
	  } else if (b.getText().equals(getString("browser"))) {
	    JFileChooser file = new JFileChooser(new File(prop.savePath));
	    file.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
	    int result = file.showSaveDialog(this);
	    if (result == JFileChooser.CANCEL_OPTION) return;
	    tf[0].setText(file.getSelectedFile().toString());
	  } else {
	    hide();
	  }
	}
    }


    /** 下載對話框 :: Download Dailog */
    class SingleDownload extends JDialog implements ActionListener {
	final JTextField[] tf = new JTextField[7];
	final JButton finish = new JButton(getString("okay"));
	final JButton cancel = new JButton(getString("cancel"));
	final JButton dir = new JButton(getString("browser"));
	final ButtonGroup bgroup = new ButtonGroup();
	final String[] label = {
		getString("single.url"), getString("single.referer"),
		getString("single.filename"), getString("save"),
		getString("retry"), getString("delay"), getString("buffer")};
	  
	String url,referer,fileName;
	int index,status;

	String savePath_;
	int maxtry_, delay_, buffer_;

	public SingleDownload(JFrame f) {
	  super(f, getString("single.title"), true);
	  initComponent();
	  setLookAndFeel(1);
	  setSize(500,300);
	  setLocationRelativeTo(null);
	  finish.addActionListener(this);
	  cancel.addActionListener(this);
	  dir.addActionListener(this);
	  windowList.add(this);
	}

	void initComponent() {
	  for (int i=0; i<tf.length; i++) tf[i] = new JTextField();
	  JPanel radioPane = new JPanel();
	  JRadioButton rb = new JRadioButton(getString("hand"));
	  rb.setActionCommand("hands");
	  bgroup.add(rb);
	  radioPane.add(rb);
	
	  rb = new JRadioButton(getString("queue"),true);
	  rb.setActionCommand("quere");
	  bgroup.add(rb);
	  radioPane.add(rb);
	
	  g.insets = new Insets(2,2,2,2);
	  g.fill = GridBagConstraints.BOTH;
	  g.weighty = 1.0; g.weightx = 1.0;
	  
	  Border bo = BorderFactory.createEtchedBorder();
	  JPanel main = new JPanel(new GridBagLayout());
	  JPanel pane = new JPanel(new GridBagLayout());
	  JPanel paneBottom = new JPanel(new GridBagLayout());
	  pane.setBorder(new TitledBorder(bo, getString("preferences")));
	  paneBottom.setBorder(bo);
	  
	  /*	    GridBagConstraints g, JPanel p, JComponent com,
	     *	    int gridwidth, int gridheight, int x, int y
	     */
	  g.weightx = 0.1;  addCom(g, pane, new JLabel(label[0],
			    SwingConstants.RIGHT),1,1 , 0,0);
	  g.weightx = 0.9;  addCom(g, pane, tf[0], 3,1 , 1,0);
	  g.weightx = 0.1;  addCom(g, pane, new JLabel(label[1],
			    SwingConstants.RIGHT),1,1 , 0,1);
	  g.weightx = 0.9;  addCom(g, pane, tf[1], 3,1 , 1,1);
	  g.weightx = 0.1;  addCom(g, pane, new JLabel(label[2],
			    SwingConstants.RIGHT),1,1 , 0,2);
	  g.weightx = 0.9;  addCom(g, pane, tf[2], 3,1 , 1,2);
	
	  g.weightx = 0.1;  addCom(g, pane, new JLabel(label[3],
			    SwingConstants.RIGHT),1,1 , 0,3);
	  g.weightx = 0.8;  addCom(g, pane, tf[3], 2,1 , 1,3);
	  g.weightx = 0.01; addCom(g, pane, dir,   1,1 , 3,3);
	
	  g.weightx = 0.1;  addCom(g, pane, new JLabel(label[4],
				SwingConstants.RIGHT),1,1 , 0,4);
	  g.weightx = 0.4;  addCom(g, pane, tf[4], 3,1 , 1,4);
	  g.weightx = 0.1;  addCom(g, pane, new JLabel(label[5],
				SwingConstants.RIGHT),1,1 , 0,5);
	  g.weightx = 0.4;  addCom(g, pane, tf[5], 3,1 , 1,5);
	  
	  g.weightx = 0.1;  addCom(g, pane, new JLabel(label[6],
				SwingConstants.RIGHT),1,1 , 0,6);
	  g.weightx = 0.4;  addCom(g, pane, tf[6], 3,1 , 1,6);
	    //g.weightx = 0.6;  addCom(g, pane, radioPane, 2,1 , 2,5);

	  g.weightx = 0.8;  addCom(g, paneBottom, radioPane, 1,1,0,0);
	  g.weightx = 0.1;  addCom(g, paneBottom, finish, 1,1 ,1,0);
	  g.weightx = 0.1;  addCom(g, paneBottom, cancel, 1,1 ,2,0);

	  g.weighty = 0.9;  addCom(g, main, pane, 1, 1, 0, 0);
	  g.weighty = 0.1;  addCom(g, main, paneBottom, 1, 1, 0, 1);

	  setContentPane(main);
	}

	JTextField getJTextField(final String str) {
	  for (int i=0; i<label.length; i++)
	  if (label[i].equals(str)) return tf[i];
	  return null;
	}

	public final void open() {
	  String clip = getClipboardText();
	  if (clip == null) {
	    tf[0].setText("");
	    tf[1].setText("");
	    tf[2].setText("");
	  } else {
	    if (!clip.equals("") && clip.split("\n").length == 1) {
	      if (clip.indexOf("http") != -1 ) {
		SeedURL u = SeedURL.newInstance(clip);
		if (u != null) {
		  tf[0].setText(u.getURL());
		  if (u instanceof SeedHttpURL)
		    tf[1].setText(((SeedHttpURL)u).getReferer());
		    tf[2].setText(u.getFileName());
		}
	      }
	    }
	  }
	  tf[3].setText(prop.savePath); 
	  tf[4].setText(prop.maxtry+""); 
	  tf[5].setText(prop.delay+"");
	  tf[6].setText(prop.buffer+"");
	  tf[0].setEditable(true);
	  tf[0].requestFocus();
	  finish.setText(getString("okay"));
	  this.show();
	}

	public final void openModifies(int index) {
	  this.index = index;
	  Seed seed = runtime.get(index);
	  SeedFileStatus s = seed.getFileStatus();
	  tf[0].setText(seed.getURL().getURL());
	  tf[0].setEditable(false);
	  if (seed.getURL() instanceof SeedHttpURL)
	    tf[1].setText(((SeedHttpURL)seed.getURL()).getReferer());
	  tf[2].setText(s.fileName);
	  tf[3].setText(s.saveDir);
	  tf[4].setText(seed.getMaxtry()+"");
	  tf[5].setText(seed.getDelay()+"");
	  tf[6].setText(seed.getBuffer()+"");
	  finish.setText(getString("update"));
	  tf[0].requestFocus();
	  tf[0].selectAll();
	  this.show();
	}
	  
	private void updateText() {
	  url = tf[0].getText();
	  referer = tf[1].getText();
	  fileName = tf[2].getText();
	  savePath_ = tf[3].getText();
	  maxtry_ = Integer.parseInt(tf[4].getText());
	  delay_ = Integer.parseInt(tf[5].getText());
	  buffer_ = Integer.parseInt(tf[6].getText());
	  if (bgroup.getSelection().getActionCommand().equals("hands"))
	    status = Seed.STOP;
	  else
	    status = Seed.QUEUE;
	}

	public void actionPerformed(ActionEvent ae) {
	  updateText();
	  JButton bt = (JButton) ae.getSource();
	  if (bt.getText().equals("...")) {
	    JFileChooser file = new JFileChooser(new File(prop.savePath));
	    file.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
	    int result = file.showSaveDialog(this);
	    if (result == JFileChooser.CANCEL_OPTION) return;
	      tf[3].setText(file.getSelectedFile().toString());
	  } else if (bt.getText().equals(getString("cancel"))) {
	    this.hide();
	  } else {
	    if (!url.equals("") && !prop.savePath.equals("")) {
	      if (finish.getText().equals(getString("okay")))
		addURL(url,referer,delay_,maxtry_,status,savePath_,fileName,
		buffer_);
		else
		updateURL(index,savePath_,referer,delay_,maxtry_,status,
		fileName,buffer_);
		this.hide();
	    }
	  }
	}
    }


    /** 多行下載對話框 :: Multi Download Dialog */
    class MultiDownload extends JDialog implements ActionListener {
	JTextArea url = new JTextArea();
	JTextField tf[] = new JTextField[5];
	JButton bt[] = new JButton[5];
	int maxtry_, delay_, buffer_;
	String dir, referer_;
	
	public MultiDownload(JFrame frame) {
	  super(frame, getString("multi.title"),true);
	  initComponent();
	  setLookAndFeel(1);
	  setSize(500,400);
	  setLocationRelativeTo(null);
	  windowList.add(this);
	}

	void initComponent() {
	  String info[] = {
		getString("multi.title"), getString("preferences"),
		getString("bytes"),getString("times"), getString("millis")};
//		"多行列下載","屬性","位元組","次","毫秒"};
	  String label[] = {
		getString("save"),getString("buffer"),getString("retry"),
		getString("delay"), getString("browser"), getString("clear"),
		getString("paste"), getString("okay"), getString("cancel")};
//		"儲存目錄","緩衝大小","重試次數","延遲時間",
//		"...","清除","貼上","完成","取消"};
	  for (int i=0; i<tf.length; i++) tf[i] = new JTextField();
	  for (int j=0; j<bt.length; j++) {
	    bt[j] = new JButton(label[4+j]);
	    bt[j].addActionListener(this);
	  }

	  Border bo = BorderFactory.createEtchedBorder();
	  JPanel topPane = new JPanel(new GridBagLayout());
	  JPanel propPane = new JPanel(new GridBagLayout());
	  JPanel buttonPane = new JPanel(new GridBagLayout());

	  topPane.setBorder(new TitledBorder(bo,info[0]));
	  propPane.setBorder(new TitledBorder(bo,info[1]));
	  buttonPane.setBorder(bo);

	  g.insets = new Insets(2,2,2,2);
	  g.fill = GridBagConstraints.BOTH;
	  g.weighty = 1.0; g.weightx = 1.0;
	    
	    /*	    GridBagConstraints g, JPanel p, JComponent com,
	     *	    int gridwidth, int gridheight, int x, int y
	     */

	  addCom(g,topPane,new JScrollPane(url),1,1,0,0);
	    
	  g.weightx = 0.1; 
	  addCom(g,propPane,new JLabel(label[0],SwingConstants.RIGHT),1,1,0,0);
	  g.weightx = 0.8;
	  addCom(g,propPane,tf[0],1,1,1,0);
	  g.weightx = 0.1;
	  addCom(g,propPane,bt[0],1,1,2,0);
	    
	  g.weightx = 0.1;
	  addCom(g,propPane,new JLabel(label[1],SwingConstants.RIGHT),1,1,0,1);
	  g.weightx = 0.8;
	  addCom(g,propPane,tf[1],1,1,1,1);
	  g.weightx = 0.1;
	  addCom(g,propPane,new JLabel(info[2],SwingConstants.LEFT),1,1,2,1);
	  
	    
	  g.weightx = 0.1;
	  addCom(g,propPane,new JLabel(label[2],SwingConstants.RIGHT),1,1,0,2);
	  g.weightx = 0.8;
	  addCom(g,propPane,tf[2],1,1,1,2);
	  g.weightx = 0.1;
	  addCom(g,propPane,new JLabel(info[3],SwingConstants.LEFT),1,1,2,2);
	    
	  g.weightx = 0.1;
	  addCom(g,propPane,new JLabel(label[3],SwingConstants.RIGHT),1,1,0,3);
	  g.weightx = 0.8;
	  addCom(g,propPane,tf[3],1,1,1,3);
	  g.weightx = 0.1;
	  addCom(g,propPane,new JLabel(info[4],SwingConstants.LEFT),1,1,2,3);

	  g.weightx = 0.1;
	  addCom(g,propPane,new JLabel("Referer",SwingConstants.RIGHT),1,1,0,4);
	  g.weightx = 0.8;
	  addCom(g,propPane,tf[4],1,1,1,4);

	  g.weightx = 0.6;
	  addCom(g,buttonPane,new JLabel(""),1,1,0,0);
	  g.weightx = 0.1;
	  addCom(g,buttonPane,bt[1],1,1,1,0);
	  g.weightx = 0.1;
	  addCom(g,buttonPane,bt[2],1,1,2,0);
	  g.weightx = 0.1;
	  addCom(g,buttonPane,bt[3],1,1,3,0);
	  g.weightx = 0.1;
	  addCom(g,buttonPane,bt[4],1,1,4,0);

	  g.weightx = 1.0; g.weighty = 1.0;

	  g.insets = new Insets(1,1,1,1);
	  JPanel main = new JPanel(new GridBagLayout());
	  g.weighty = 1.0;
	  addCom(g,main,topPane,1,1,0,0);
	  g.weighty = 0.1;
	  addCom(g,main,propPane,1,1,0,1);
	  g.weighty = 0.1;
	  addCom(g,main,buttonPane,1,1,0,2);

	  setContentPane(main);
	}

	public void open() {
	  tf[0].setText(prop.savePath);
	  tf[1].setText(prop.buffer+"");
	  tf[2].setText(prop.maxtry+"");
	  tf[3].setText(prop.delay+"");
	  show();
	}

	private void setText() {
	  dir = tf[0].getText();
	  buffer_ = Integer.parseInt(tf[1].getText());
	  maxtry_ = Integer.parseInt(tf[2].getText());
	  delay_ = Integer.parseInt(tf[3].getText());
	  referer_ = tf[4].getText();
	}

	public void actionPerformed(ActionEvent ae) {
	  JButton bt = (JButton) ae.getSource();
	  String text = bt.getText();
	  if (text.equals(getString("paste"))) {
	    String str = getClipboardText();
	    if (str != null) url.append(str+"\n");
	  } else if (text.equals(getString("clear"))) {
	      url.setText("");
	  } else if (text.equals(getString("okay"))) {
	      setText();
	      addMultiURL(url.getText());
	      hide();
	  } else if (text.equals(getString("cancel"))) {
	      hide();
	  } else if (text.equals(getString("browser"))) {
	      JFileChooser file = new JFileChooser(new File(prop.savePath));
	      file.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
	      int result = file.showSaveDialog(this);
	      if (result == JFileChooser.CANCEL_OPTION) return;
	      tf[0].setText(file.getSelectedFile().toString());
	  }
	}

	void addMultiURL(final String str) {
	  if (str == null) return;
	  if (str.equals("")) return;
	  new Thread() {
	    public void run() {
	      String[] line = str.split("\n");
	      String s = "";
	      for (int i=0;i<line.length; i++) {
		s = line[i].trim();
		if (!s.equals("")) {
		  Seed seed = 
		  Seed.newInstance(s,"",dir,Seed.QUEUE,maxtry_,delay_,buffer_);
		  if (!referer_.equals("")) {
		      ((SeedHttpURL) seed.getURL()).setReferer(referer_);
		  }
		  runtime.add(seed);
		}
	      }
	      table.fireTable();
	      runtime.wakeup();
	    }
	  }.start();
	}
    }



    /** 種子表格 :: Seed Table */
    class SeedTable extends JTable {
	
	JPopupMenu p;
	MouseAdapter mouse;
	AbstractTableModel model;
	TableCellRenderer percentRenderer;
	TableCellRenderer fileNameRenderer;

	public SeedTable() {
	  initComp();
	  setModel(model);
	  setAutoResizeMode(JTable.AUTO_RESIZE_ALL_COLUMNS);
	  setSelectionMode(ListSelectionModel.MULTIPLE_INTERVAL_SELECTION);
	  setShowGrid(false);
	  addMouseListener(mouse);
	  for (int i=0; i< getColumnModel().getColumnCount(); i++) {
	      int percent = Integer.parseInt(tableWidth.split(" ")[i]);
	      getColumnModel().getColumn(i).setPreferredWidth(
		    (prop.frame_width/100*percent));
	  }
	  getColumnModel().getColumn(0).setCellRenderer(fileNameRenderer);
	  getColumnModel().getColumn(3).setCellRenderer(percentRenderer);
	}

	void initComp() {
	  p = new JPopupMenu("Menu");
	  p.add(startRangeAct);
	  p.add(stopRangeAct);
	  p.addSeparator();
	  p.add(startAllAct);
	  p.add(stopAllAct);
	  p.addSeparator();
	  p.add(rmAct);
	  p.add(removeFinish);
	  p.add(rmFileAct);
	  p.add(rmFileAndSeedAct);
 	  p.addSeparator();
 	  p.add(setToZero);
	  p.add(setToZeroRemove);
	  p.add(setErrToZero);
	  p.add(setErrToZeroRemove);
	  p.addSeparator();
	  p.add(modifiesAct);

	  mouse = new MouseAdapter() {
	    public void mouseClicked(MouseEvent me) {
	      if (me.getClickCount() == 2)
		  single.openModifies(table.getSelectedRow());
	      if (table.getSelectedRows().length == 1)
		  logTable.changeSeedIndex(table.getSelectedRow());
	      else
		  logTable.changeSeedIndex(-1);
	    }
	    public void mousePressed(MouseEvent me) {
	      checkPopup(me);
	    }
	    public void mouseReleased(MouseEvent me) {
	      checkPopup( me );
	      if (table.getSelectedRows().length > 1)
		  logTable.changeSeedIndex(-1);
	    }
	  };

	  fileNameRenderer = new TableCellRenderer() {
	    public Component getTableCellRendererComponent(JTable table, 
		    Object value,
		    boolean isSelected,
		    boolean hasFocus, int row, int column) {
			/** fileName field */
		if (runtime.getSize() == 0) return null;
		JLabel lb = new JLabel();
		lb.setText((String) value);
		lb.setBackground((isSelected?selectionColor:table.getBackground()));
		lb.setEnabled(table.isEnabled());
		lb.setOpaque(true);
		int status = runtime.get(row).getStatus();
		if (status == Seed.QUEUE) lb.setIcon(queue);
		else if (status == Seed.STOP) lb.setIcon(wait);
		else if (status == Seed.RUN) lb.setIcon(run);
		else if (status == Seed.FINISH) lb.setIcon(finish);
		else if (status == Seed.PARTIAL) lb.setIcon(err);
		return lb;
	    }
	  };

	  percentRenderer = new TableCellRenderer() {
	    public Component getTableCellRendererComponent(JTable table, 
		    Object value,
		    boolean isSelected,
		    boolean hasFocus, int row, int column) {
			/** fileName field */
		if (runtime.getSize() == 0) return null;
		JProgressBar jb = new JProgressBar(0, 100);
		jb.setOrientation( JProgressBar.HORIZONTAL );
		jb.setStringPainted( true );
		jb.setBorderPainted(false);
		jb.setValue(((Integer)value).intValue());
		jb.setForeground(BLUE);
		jb.setBackground((isSelected?selectionColor:table.getBackground()));
		return jb;
	    }
	  };

	  model = new AbstractTableModel() {
	    public int getColumnCount() {
		return tableLabel.split(" ").length;
	    }
	    
	    public int getRowCount() {
		if (runtime.getSize() == 0) return -1;
		return runtime.getSize();
	    }

	    public Object getValueAt(int rowIndex, int columnIndex) {
		if (runtime.getSize() == 0) return null;
		Seed seed = runtime.get(rowIndex);
		SeedFileStatus s = seed.getFileStatus();
		switch(columnIndex) {
		    case 0: return s.fileName;
		    case 1: return new Long(s.contentSize);
		    case 2: return new Long(s.reads);
		    case 3: return new Integer(s.percent);
		    case 4: return s.timeLabel;
		    case 5: return s.timeLeftLabel;
		    case 6: return s.speedLabel;
		}
		return null;
	    }
	    public boolean isCellEditable(int rowIndex, int columnIndex) {
		return false;
	    }

	    public String getColumnName(int num) {
		return tableLabel.split(" ")[num];
	    }
    
	    public Class getColumnClass(int columnIndex) {
		if (runtime.getSize() == 0) return null;
		return getValueAt(0,columnIndex).getClass();
	    }
	  };
	}
    
	void checkPopup(MouseEvent me) {
	  if (me.isPopupTrigger()) {
	      if (getSelectedRows().length == 0) return;
	      p.show(table, me.getX(), me.getY());
	  }
	}

	/*
	public void updateRunningSeed(final String speed) {
	  int min = -1, max = 0;
	  for (int i=0; i<runtime.getSize(); i++) {
	    Seed seed = runtime.get(i);
	    if (seed.getStatus() == Seed.RUN) {
	      if (min == -1)
		  min = i;
	      else
		  max = i;
	    }
	  }
	  if (min == -1) min = 0;
	  if (min > max) {
	      int tmp = min;
	      min = max;
	      max = tmp;
	  }
          final int fmin = min, fmax = max;
          SwingUtilities.invokeLater(new Runnable() {
	    public void run() {
		model.fireTableRowsUpdated(fmin, fmax);
		statusbar.update(speed);
	    }
	  });
	}
    
	public void updateAllTable() {
	  int[] rows = new int[runtime.getSize()];
	  for (int i=0; i<rows.length; i++) rows[i] = i;
	  updateTableRange(rows);
	}

	public void updateTableRange(final int single) {
	  if (single != -1) {
	  SwingUtilities.invokeLater(new Runnable() {
	    public void run() {
		model.fireTableRowsUpdated(0, single);
	    }
	  });
	}
      }

      public void updateTableRange(int[] rows) {
	int min = 0, max = 0;
	for (int i=0; i<rows.length; i++) {
	  Seed seed = runtime.get(rows[i]);
	  if (i == 0)
	      min = rows[i];
	  else
	      max = rows[i];
	}
      
        if (min > max) {
	    int tmp = min;
	    min = max;
	    max = tmp;
        }

	final int fmin = min, fmax = max;
	SwingUtilities.invokeLater(new Runnable() {
	  public void run() {
	    model.fireTableRowsUpdated(fmin, fmax);
	  }
	});
      }

      }
      */

      public void updateTableRange(final int[] rows) {
	SwingUtilities.invokeLater(new Runnable() {
	  public void run() {
	    System.out.println(rows[0]+"/"+rows[1]);
	    model.fireTableRowsUpdated(rows[0], rows[1]);
	  }
	});
      }
      
      public void fireTable() {
	SwingUtilities.invokeLater(new Runnable() {
	  public void run() {
	    model.fireTableDataChanged();
	  }
        });
      }
    }


    String tableLabel = new StringBuffer()
	    .append(getString("table.name"))
	    .append(" ")
	    .append(getString("table.size"))
	    .append(" ")
	    .append(getString("table.complete"))
	    .append(" ")
	    .append(getString("table.percent"))
	    .append(" ")
	    .append(getString("table.costtime"))
	    .append(" ")
	    .append(getString("table.timeleft"))
	    .append(" ")
	    .append(getString("table.speed")).toString();
      
      final public String tableWidth =
	    "30 12 12 17 10 10 10";
	    
      final Color selectionColor = new Color(210,224,250,255);
      
      final ImageIcon wait = new ImageIcon(
				ClassLoader.getSystemClassLoader().
				    getResource("img2/table/pause.png"));
      final ImageIcon queue = new ImageIcon(
				ClassLoader.getSystemClassLoader().
				    getResource("img2/table/queue.png"));
      final ImageIcon run = new ImageIcon(
				ClassLoader.getSystemClassLoader().
				    getResource("img2/table/connected.png"));
      final ImageIcon finish = new ImageIcon(
				ClassLoader.getSystemClassLoader().
				    getResource("img2/table/finish.png"));
      final ImageIcon err = new ImageIcon(
				ClassLoader.getSystemClassLoader().
				    getResource("img2/table/partial.png"));
      
      final ImageIcon put = new ImageIcon(
				ClassLoader.getSystemClassLoader().
				    getResource("img/mini_up.gif"));
      final ImageIcon get = new ImageIcon(
				ClassLoader.getSystemClassLoader().
				    getResource("img/mini_down.gif"));
      final ImageIcon warning = new ImageIcon(
				ClassLoader.getSystemClassLoader().
				    getResource("img/mini_i.gif"));
      final ImageIcon error = new ImageIcon(
				ClassLoader.getSystemClassLoader().
				    getResource("img/mini_err.gif"));

    final String logLabel =
		new StringBuffer()
		.append(getString("logtable.time"))
		.append(" ")
		.append(getString("logtable.note")).toString();
    final String logWidth = "36 65";


    /** 記錄表格 :: Log Table */
    class SeedLogTable extends JTable {
      Seed seed;
      AbstractTableModel logModel;
      TableCellRenderer renderer;
      public SeedLogTable() {
	logModel = new AbstractTableModel() {
	    public final int getColumnCount() {
		return logLabel.split(" ").length;
	    }
	    public final int getRowCount() {
		if (seed == null) return -1;
		return seed.getLogger().size();
	    }
	    public final Object getValueAt(int rowIndex, int columnIndex) {
		if (seed == null) return null;
		SeedLogMsg log = seed.getLogger().get(rowIndex);
		if (columnIndex == 0) {
		    return log.getDate();
		} 
		if (columnIndex == 1) {
		    return log.getMsg();
		}
		return null;
	    }
	    public final boolean isCellEditable(int rowIndex, int columnIndex) {
		return false;
	    }
	    public final String getColumnName(int num) {
		return logLabel.split(" ")[num];
	    }
	    public final Class getColumnClass(int columnIndex) {
		if (seed == null) return null;
		return getValueAt(0,columnIndex).getClass();
	    }
	};
	
	renderer = new TableCellRenderer() {
	    public final Component getTableCellRendererComponent(JTable table, 
		Object value,boolean isSelected, boolean hasFocus,
		int row,int column) {
		if (seed == null) return null;
		int status = seed.getLogger().get(row).getType();
		JLabel lb = new JLabel();
		lb.setText(value.toString());
		if (status == 1) {
		    lb.setForeground(GREEN);
		    lb.setBackground(bGREEN);
		    if (column == 0) lb.setIcon(put);
		} else if (status == 2) { 
		    lb.setForeground(YELLOW);
		    lb.setBackground(bYELLOW);
		    if (column == 0) lb.setIcon(get);
		} else if (status == 3) { 
		    lb.setForeground(BLUE);
		    lb.setBackground(bBLUE);
		    if (column == 0) lb.setIcon(warning);
		} else if (status == 4) {
		    lb.setForeground(RED);
		    lb.setBackground(bRED);
		    if (column == 0) lb.setIcon(error);
		}
		lb.setEnabled(table.isEnabled());
		lb.setOpaque(true);
		return lb;
	    }
	};

	setModel(logModel);
	setAutoResizeMode(AUTO_RESIZE_ALL_COLUMNS);
	setSelectionMode(ListSelectionModel.MULTIPLE_INTERVAL_SELECTION);
	setShowGrid(false);
	for (int i=0; i< logModel.getColumnCount(); i++) {
	  int percent = Integer.parseInt(logWidth.split(" ")[i]);
	  getColumnModel().getColumn(i).setCellRenderer(renderer);
	  getColumnModel().getColumn(i).setPreferredWidth(
		(prop.frame_width/100*percent));
	}
      }

      public void changeSeedIndex(int index) {
	if (index == -1) seed = null;
	else seed = runtime.get(index);
	logModel.fireTableDataChanged();
      }
    }

    class LookNFeel extends AbstractAction {
	public void actionPerformed(ActionEvent e) {
	  String theme = e.getActionCommand();
	  try {
		UIManager.setLookAndFeel(theme);
		for (int i=0; i<windowList.size(); i++) {
		  Component j = (Component) windowList.get(i);
		  SwingUtilities.updateComponentTreeUI(j);
		}
	  } catch (Exception z) { z.printStackTrace(); }
	}
    }
    
    class About extends AbstractAction {
	public About() {
	  super(getString("about"),getIcon("about"));
	}

	public void actionPerformed(ActionEvent e) {
	  aboutDialog.open();
	}
    }

    class AddSingleDownload extends AbstractAction {
	public AddSingleDownload() {
	  super(getString("single"),getIcon("single"));
	}

	public void actionPerformed(ActionEvent e) {
	  single.open();
	}
    }

    class AddMultiDownload extends AbstractAction {
	public AddMultiDownload() {
	  super(getString("multi"),getIcon("multi"));
	}

	public void actionPerformed(ActionEvent e) {
	  multi.open();
	}
    }

    class SetOption extends AbstractAction {
	public SetOption() {
	  super(getString("option"),getIcon("option"));
	}

	public void actionPerformed(ActionEvent e) {
	  propDialog.open();
	}
    }

    class ModifiesDownload extends AbstractAction {
	public ModifiesDownload() {
	  super(getString("edit"),getIcon("edit"));
	}

	public void actionPerformed(ActionEvent e) {
	  single.openModifies(table.getSelectedRow());
	}
    }


    class Close extends AbstractAction {
	public Close() {
	  super(getString("exit"),getIcon("exit"));
	}

	public void actionPerformed(ActionEvent e) {
	  handler.stopAllSeed();
	  close();
	}
    }
    

    /**
      *   一些動作 
		StartRange, StartAll
		StopRange, StopAll
		RemoveSeedRange, RemoveSeedAll
		RemoveFileRange, RemoveFileAll
		RemoveSeedFileRange, RemoveSeedFileAll
    */

    class StartRange extends AbstractAction {
	public StartRange() {
	  super(getString("start"),getIcon("start"));
	}

	public void actionPerformed(ActionEvent e) {
	  new Thread() {
	    public void run() {
		int[] rows = table.getSelectedRows();
		handler.startSeedRange(rows);
		runtime.wakeup();
		status.updateTableRange(rows);
	    }
	  }.start();
	}
    }

    class StartAll extends AbstractAction {
	public StartAll() {
	  super(getString("startAll"),getIcon("startAll"));
	}

	public void actionPerformed(ActionEvent e) {
	  Thread r = new Thread() {
	    public void run() {
	      handler.startAllSeed();
	      runtime.wakeup();
	      table.updateTableRange(new int[]{0, runtime.getSize()-1});
	    }
	  };
	  r.start();
	}
    }

    class StopRange extends AbstractAction {
	public StopRange() {
	  super(getString("stop"),getIcon("stop"));
	}

	public void actionPerformed(ActionEvent e) {
	  new Thread() {
	    public void run() {
	      int[] rows = table.getSelectedRows();
	      handler.stopSeedRange(rows);
	      runtime.wakeup();
	      status.updateTableRange(rows);
	    }
	  }.start();
	}
    }

    class StopAll extends AbstractAction {
	public StopAll() {
	  super(getString("stopAll"),getIcon("stopAll"));
	}

	public void actionPerformed(ActionEvent e) {
	  new Thread() {
	    public void run() {
	      handler.stopAllSeed();
	      runtime.wakeup();
	      table.updateTableRange(new int[]{0, runtime.getSize()-1});
	    }
	  }.start();
	}
    }

    class Remove extends AbstractAction {
	public Remove() {
	  super(getString("remove"),getIcon("remove"));
	}

	public void actionPerformed(ActionEvent e) {
	  new Thread() {
	    public void run() {
	      int[] rows = table.getSelectedRows();
	      handler.removeSeedRange(rows);
	      runtime.wakeup();
	      status.fireTable();
	    }
	  }.start();
	}
    }
    
    class RemoveFinish extends AbstractAction {
	public RemoveFinish() {
	  super(getString("removefinish"),getIcon("remove"));
	}

	public void actionPerformed(ActionEvent e) {
	  new Thread() {
	    public void run() {
	      handler.removeFinish();
	      status.fireTable();
	    }
	  }.start();
	}
    }
	
    class RemoveFile extends AbstractAction {
	public RemoveFile() {
	  super(getString("removefile"),getIcon("removefile"));
	}

	public void actionPerformed(ActionEvent e) {
	  new Thread() {
	    public void run() {
	      int[] rows = table.getSelectedRows();
	      handler.removeSeedFile(rows);
	      status.updateTableRange(rows);
	    }
	  }.start();
	}
    }

     class RemoveSeedAndFile extends AbstractAction {
	public RemoveSeedAndFile() {
	  super(getString("removefileseed"),getIcon("removefileseed"));
	}

	public void actionPerformed(ActionEvent e) {
	  new Thread() {
	    public void run() {
	      int[] rows = table.getSelectedRows();
	      handler.stopSeedRange(rows);
	      handler.removeSeedFile(rows);
	      handler.removeSeedRange(rows);
	      runtime.wakeup();
	      status.fireTable();
	    }
	  }.start();
	}
    }

    class SetToZeroRange extends AbstractAction {
	public SetToZeroRange() {
	  super(getString("set2zero"));
	}

	public void actionPerformed(ActionEvent e) {
	  new Thread() {
	    public void run() {
	      int[] rows = table.getSelectedRows();
	      handler.setSeedZeroRange(rows);
	      status.updateTableRange(rows);
	    }
	  }.start();
	}
    }
    
    class SetToZeroAndRemove extends AbstractAction {
	public SetToZeroAndRemove() {
	  super(getString("set2zeroremove"));
	}

	public void actionPerformed(ActionEvent e) {
	  new Thread() {
	    public void run() {
	      int[] rows = table.getSelectedRows();
	      handler.removeSeedFile(rows);
	      handler.setSeedZeroRange(rows);
	      status.updateTableRange(rows);
	    }
	  }.start();
	}
    }
    
    class SetErrorsSeedToZero extends AbstractAction {
	public SetErrorsSeedToZero() {
	  super(getString("seterrfile2zero"));
	}

	public void actionPerformed(ActionEvent e) {
	  new Thread() {
	    public void run() {
	      handler.setErrorSeedZero();
	      table.updateTableRange(new int[]{0, runtime.getSize()-1});
	    }
	  }.start();
	}
    }
    
    class SetErrorsSeedToZeroAndRemove extends AbstractAction {
	public SetErrorsSeedToZeroAndRemove() {
	  super(getString("seterrfile2zeroremove"));
	}

	public void actionPerformed(ActionEvent e) {
	  new Thread() {
	    public void run() {
	      handler.removeErrorsFile();
	      handler.setErrorSeedZero();
	      table.updateTableRange(new int[]{0, runtime.getSize()-1});
	    }
	  }.start();
	}
    }
    
    public void addURL(String url,String referer,int delay,int maxtry,
	    int status,String dir,String fileName,int buffer) {
      Seed seed = Seed.newInstance(url,fileName,dir,status,maxtry,delay,buffer);
      if (seed.getURL() instanceof SeedHttpURL) {
	if (referer != null)
	  if (!referer.equals(""))
	    ((SeedHttpURL)seed.getURL()).setReferer(referer);
	runtime.add(seed);
	runtime.wakeup();
	table.fireTable();
      }
    }

    public void updateURL(int index,String dir,String referer,
    int delay, int maxtry,int status, String fileName,int buffer) {
      handler.updateSeed(index,dir,referer,delay,maxtry,status,fileName,buffer);
      table.updateTableRange(new int[]{index,index});
    }

    public void update() {
	synchronized(runtime.getSyncKey()) {
	    if (status.update) {
		table.updateTableRange(status.getUpdateRange());
	    } else if (status.fireTable) {
		status.getFireTable();
		table.fireTable();
	    }
	}
    }

    public void updateLabel() {
	if (status.fileNumLabelChanged)
	    statusbar.setFileNum(status.getFileNumLabel());
	if (status.fileSizeLabelChanged)
	    statusbar.setFileSize(status.getFileSizeLabel());
	if (status.fileSpeedLabelChanged)
	    statusbar.setSpeed(status.getSpeedLabel());
    }
	

    public void close() {
      prop.locationX = getX();
      prop.locationY = getY();
      prop.frame_width = getWidth();
      prop.frame_height = getHeight();
      prop.writeProperties();
      prop.writeSeedURL(runtime);
      System.exit(1);
    }

    public static void main(String[] a) {
      new JNetTransport();
    }
}
