package vq;
import java.io.*;
import java.awt.image.*;
import img.*;
import ui.*;

public class SplitBlock {

    static String HEX = "0123456789ABCDEF";

    public static void main(String[] a)
    throws IOException {

    }

    int tmp, tmpa, tmpb;
    double D0, D1;
    double THRESHOLD = 0.1;
    
    int splitW;
    int splitH;
    int fixW;
    int fixH;

    byte[][] blocks_r;
    byte[][] blocks_g;
    byte[][] blocks_b;

    int blkSize = 64;
    byte[][] training_r;
    byte[][] training_g;
    byte[][] training_b;

    int[] new_blocks_r;
    int[] new_blocks_g;
    int[] new_blocks_b;

    boolean stop = false;
    

    public int getR(int rgb) {
	return (int)((rgb>>>16)&0xff);
    }
    
    public int getG(int rgb) {
	return (int)((rgb>>>8)&0xff);
    }
    
    public int getB(int rgb) {
	return (int)((rgb)&0xff);
    }

    public int getRGB(byte r, byte g, byte b) {
	int rgb = (255&0xff)<<24 |
		(((int)r)&0xff)<<16 |
		(((int)g)&0xff)<<8 |
		(((int)b)&0xff);
	return rgb;
    }

     double rmse(BufferedImage ori, BufferedImage c) {
	double rmse  =  0.0, rmse2 = 0.0;
	int r = 0, g = 0, b = 0;
	int rgb1 = 0, rgb2 = 0;
	int times = 0;
	for (int i=0; i<ori.getHeight()/8; i++)
	for (int j=0; j<ori.getWidth()/8; j++) {
	    rmse = 0;
	    for (int ii=0; ii<8; ii++)
	    for (int jj=0; jj<8; jj++) {
		rgb1 = ori.getRGB(j*8+jj, i*8+ii);
		rgb2 = c.getRGB(j*8+jj, i*8+ii);
		rmse += Math.pow(getR(rgb1) - getR(rgb2), 2);
		r = Math.abs(getR(rgb1) - getR(rgb2));
		g = Math.abs(getG(rgb1) - getG(rgb2));
		b = Math.abs(getB(rgb1) - getB(rgb2));
	    }
	    rmse2 += Math.sqrt(rmse/64);
	    times++;
	}
	return rmse2/times;
    }

    long start;
    
    double bpp;
    String fileName;
    public SplitBlock(String fileName,  int splitW, int splitH, int blkSize, double threshold, Log log)
    throws IOException {
	THRESHOLD = threshold;
	bpp = ((Math.log(blkSize)/Math.log(2))/(splitW*splitH)) * 3;

	this.fileName = fileName;
	BufferedImage bi = BMP.read(new FileInputStream(fileName));
	start = System.currentTimeMillis();
	this.blkSize = blkSize;
	this.splitW = splitW;
	this.splitH = splitH;
	fixW = bi.getWidth()/splitW;
	fixH = bi.getHeight()/splitH;
	blocks_r = new byte[(fixW*fixH)][splitW*splitH];
	blocks_g = new byte[(fixW*fixH)][splitW*splitH];
	blocks_b = new byte[(fixW*fixH)][splitW*splitH];
	
	log.println("Img W: "+bi.getWidth()+"\nImg H: "+bi.getHeight());
	log.println("splitW: "+splitW+"\nsplitH: "+splitH);
	log.println("fixW: "+fixW+"\nfixH: "+fixH);
	log.println("block row size: "+((fixW*fixH))+
		"\nblock column size: "+(splitW*splitH));
	
	log.print("Spliting Block ... ... ");
	for (int h=0; h<fixH; h++) {
	  for (int w=0; w<fixW; w++) {
	    for (int sh=0; sh<splitH; sh++) {
		for (int sw=0; sw<splitW; sw++) {
		    try {
		    tmp = bi.getRGB(w*splitW+sw, h*splitH+sh);
		    blocks_r[h*fixW+w][sh*splitW+sw] = (byte) getR(tmp);
		    blocks_g[h*fixW+w][sh*splitW+sw] = (byte) getG(tmp);
		    blocks_b[h*fixW+w][sh*splitW+sw] = (byte) getB(tmp);
		    } catch (java.lang.ArrayIndexOutOfBoundsException e) {
			System.out.println("\nbreak poing:\nh:"+h+" w:"+w+
				"\nsh:"+sh+" sw:"+sw);
			e.printStackTrace();
			return;
		    }
		}
	    }
	  }
	}
	log.println("done!");
	

	training_r = new byte[blkSize][splitW*splitH];
	training_g = new byte[blkSize][splitW*splitH];
	training_b = new byte[blkSize][splitW*splitH];
	
	log.print("Generating Codebook ... ...");
	boolean contain[] = new boolean[blocks_r.length];
	for (int i=0; i<blkSize; i++) {
	    while(contain[tmp=(int)(Math.random()*blocks_r.length)]) {
		;
	    }
	    training_r[i] = blocks_r[tmp];
	    training_g[i] = blocks_g[tmp];
	    training_b[i] = blocks_b[tmp];
	    contain[tmp] = true;
	}
	log.println("done!\n");



	new_blocks_r = new int[(fixW*fixH)];
	new_blocks_g = new int[(fixW*fixH)];
	new_blocks_b = new int[(fixW*fixH)];

	stop = false;
	D0 = 0;
	D1 = 0;
	while(!stop) {
	    D1 = 0;
	    for (int i=0; i<blocks_r.length; i++) {
		new_blocks_r[i] = training(blocks_r[i], training_r);
	    }
	    D1 /= blocks_r.length;
	    log.println("R_D0: "+D0+"\tR_D1:"+D1);
	    
	    if (Math.abs((D1-D0))/D1 < THRESHOLD) {
		stop = true;
	    } else {
		D0 = D1;
		retraining(blocks_r, new_blocks_r, training_r);
	    }
	}


	stop = false;
	D0 = 0;
	D1 = 0;
	while(!stop) {
	    D1 = 0;
	    for (int i=0; i<blocks_g.length; i++) {
		new_blocks_g[i] = training(blocks_g[i], training_g);
	    }
	    D1 /= blocks_g.length;
	    log.println("G_D0: "+D0+"\tG_D1:"+D1);
	    if (Math.abs((D1-D0))/D1 < THRESHOLD) {
		stop = true;
	    } else {
		D0 = D1;
		retraining(blocks_g, new_blocks_g, training_g);
	    }
	}



	stop = false;
	D0 = 0;
	D1 = 0;
	while(!stop) {
	    D1 = 0;
	    for (int i=0; i<blocks_b.length; i++) {
		new_blocks_b[i] = training(blocks_b[i], training_b);
	    }
	    D1 /= blocks_b.length;
	    log.println("B_D0: "+D0+"\tB_D1:"+D1);
	    if (Math.abs((D1-D0))/D1 < THRESHOLD) {
		stop = true;
	    } else {
		D0 = D1;
		retraining(blocks_b, new_blocks_b, training_b);
	    }
	}

	start = System.currentTimeMillis()-start;
	writeFile(bi, log);
    }

    int total = 0;
    int tmpblock[];
    void retraining(byte[][] blocks, int[] new_blocks, byte[][] training) {
	for (int i=0; i<training.length; i++) {
	    tmpblock = new int[splitH*splitW];
	    total = 0;
	    for (int j=0; j<new_blocks.length; j++) {
		if (new_blocks[j] == i) {
		    combination(blocks[j], tmpblock);
		    total++;
		}
	    }

	    for (int k=0; k<training[i].length && total > 0; k++) {
		training[i][k] = (byte)((tmpblock[k]/total)&0xff);
	    }
	}
    }

    void combination(byte[] blocks, int[] tmpblock) {
	int za = 0;
	for (int i=0; i<blocks.length; i++) {
	    za = ((int) blocks[i]) & 0xff;
	    tmpblock[i] += za;
	}
    }
    
    int training(byte[] blocks, byte[][] training) {
	int min = 200000000, min_index = 0;
	for (int j=0; j<training.length; j++) {
	    tmp = getMinVal(blocks, training[j]);
	    if (tmp < min) {
		min = tmp;
		min_index = j;
	    }
	}

	D1 += min;
	return min_index;
    }

    int getMinVal(byte[] block, byte[] train) {
	int tmpc=0;
	for (int i=0; i<train.length; i++) {
	    tmpa = ((int) block[i]) & 0xff;
	    tmpb = ((int) train[i]) & 0xff;
	    tmpc += Math.pow(tmpa-tmpb, 2);
	}
	return tmpc;
    }

    public void writeFile(final BufferedImage bi, Log log)
    throws IOException {
	final BufferedImage b2i = 
	new BufferedImage((int)((fixW*splitW)), (int)((fixH*splitH)), BufferedImage.TYPE_INT_RGB);

	for (int h=0; h<fixH; h++) {
	for (int w=0; w<fixW; w++) {
	for (int sh=0; sh<splitH; sh++) {
	    for (int sw=0; sw<splitW; sw++) {
		byte r = training_r[new_blocks_r[h*fixW+w]][sh*splitW+sw];
		byte g = training_g[new_blocks_g[h*fixW+w]][sh*splitW+sw];
		byte b = training_b[new_blocks_b[h*fixW+w]][sh*splitW+sw];
		int rgb = getRGB(r, g, b);
		b2i.setRGB(w*splitW+sw, h*splitH+sh, rgb);
	    }
	}
	}
	}

	int totalbits = (int)(bpp*bi.getWidth()*bi.getHeight());
	double ratio = (bi.getWidth()*bi.getHeight()*8*3)/totalbits;
//	System.out.println(zz+"\t\t"+(bpp*bi.getWidth()*bi.getHeight()));

	final String s = "SplitSize:"+splitW+"x"+splitH+" CodeBook:"+blkSize+" threshold:"+THRESHOLD+" "+"BPP:"+bpp+"bits ratio:"+ratio+" rmse:"+rmse(bi, b2i)+" time:"+start+"ms ";
	new Thread(new Runnable() {
	    public void run() {
		new ui.Show(s,b2i);
	    }
	}).start();
	log.println("File Name: "+fileName);
	log.println("Code Book Size: "+blkSize);
	log.println("BPP: "+bpp+" bits");
	log.println("TIME: "+start+"ms");
	log.println("RMSE: "+rmse(bi, b2i));
	log.println("- - - - -\n");
	
//	BMPWriter.writeImg(b2i, "out/"+fileName+"_"+splitW+"x"+splitH+"_"+blkSize+".bmp");
    }			    
/**
    ¢u	4   ¢q	4   ¢q	4   ¢t
    ¢z¢s¢s¢s¢s¢s¢s¢s¢s¢s¢s¢s¢{	¢s
    ¢u¢q¢q¢q¢q¢q¢q¢q¢q¢q¢q¢q¢t
    ¢u¢q¢q¢q¢q¢q¢q¢q¢q¢q¢q¢q¢t	4
    ¢u¢q¢q¢q¢q¢q¢q¢q¢q¢q¢q¢q¢t	
    ¢u¢q¢q¢q¢q¢q¢q¢q¢q¢q¢q¢q¢t	¢q
    ¢u¢q¢q¢q¢q¢q¢q¢q¢q¢q¢q¢q¢t
    ¢u¢q¢q¢q¢q¢q¢q¢q¢q¢q¢q¢q¢t	4
    ¢u¢q¢q¢q¢q¢q¢q¢q¢q¢q¢q¢q¢t
    ¢u¢q¢q¢q¢q¢q¢q¢q¢q¢q¢q¢q¢t	¢q
    ¢u¢q¢q¢q¢q¢q¢q¢q¢q¢q¢q¢q¢t
    ¢u¢q¢q¢q¢q¢q¢q¢q¢q¢q¢q¢q¢t	4
    ¢u¢q¢q¢q¢q¢q¢q¢q¢q¢q¢q¢q¢t
    ¢|¢r¢r¢r¢r¢r¢r¢r¢r¢r¢r¢r¢}	¢r
*/


}

