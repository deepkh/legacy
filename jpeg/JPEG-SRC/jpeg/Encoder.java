/** JPEG Encoder implements by deepkh@gmail.com 2006-11-27 */
package jpeg;
import javax.imageio.plugins.jpeg.JPEGQTable;
import javax.imageio.plugins.jpeg.JPEGHuffmanTable;
import java.io.*;
import java.awt.image.*;
import img.*;
import ui.Log;

public class Encoder {
    
    /** DCT 系數矩陣 */
    public static double DCT[][][][];

    /** 亮度量化表 */
    public static int Luminance[] = new int[] {
        16, 11, 10, 16, 24, 40, 51, 61,
        12, 12, 14, 19, 26, 58, 60, 55,
        14, 13, 16, 24, 40, 57, 69, 56,
        14, 17, 22, 29, 51, 87, 80, 62,
        18, 22, 37, 56, 68, 109, 103, 77,
        24, 35, 55, 64, 81, 104, 113, 92,
        49, 64, 78, 87, 103, 121, 120, 101,
        72, 92, 95, 98, 112, 100, 103, 99
    };

    /** 色差量化表 */
    public static int Chrominance[] = new int[] {
        17,18,24,47,99,99,99,99,
        18,21,26,66,99,99,99,99,
        24,26,56,99,99,99,99,99,
        47,66,99,99,99,99,99,99,
        99,99,99,99,99,99,99,99,
        99,99,99,99,99,99,99,99,
        99,99,99,99,99,99,99,99,
        99,99,99,99,99,99,99,99
    };


    /** 計算量化品質。從 1 ~ 100 */
    public static int[][] getQualityTab(int q) {
	int Q[][] = new int[2][64];
	if (q <= 0) q = 1;
	if (q > 100) q = 100;
	if (q < 50)
	    q = 5000 / q;
	else
	    q = 200 - q*2;
	for (int i=0; i<64; i++) {
	    Q[0][i] = (int)(Luminance[i] * q)/100;
	    Q[1][i] = (int)(Chrominance[i] * q)/100;
	    if (Q[0][i] < 1) Q[0][i] = 1;
	    if (Q[1][i] < 1) Q[1][i] = 1;
	    if (Q[0][i] > 255) Q[0][i] = 255;
	    if (Q[1][i] > 255) Q[1][i] = 255;
	}
	return Q;
    }
    
    
    static float C(int u) {
	return (u == 0) ? (float)(1/Math.sqrt(2)) : 1.0f;
    }

    /** initial DCT coefficient matrix */
    public static void init() {
	long timeStart = System.currentTimeMillis();
	if (DCT != null) return;
	DCT = new double[8][8][8][8];
	for (int ii=0; ii<8; ii++)
	for (int jj=0; jj<8; jj++) {
	    for (int ii2=0; ii2<8; ii2++) {
		for (int jj2=0; jj2<8; jj2++) {
		    DCT[ii][jj][ii2][jj2] = ((float)((C(ii)*C(jj))/4) * Math.cos(((2*ii2+1)*ii*Math.PI)/16) *
				    Math.cos(((2*jj2+1)*jj*Math.PI)/16));
		}
	    }
	}
    }

    /** ZigZag Scan Position */
    static int[] ZZ = new int[] {
	0,1,8,16,9,2,3,10,17,24,32,25,18,11,4,5,12,19,26,33,40,48,41,34,27,20,13,6,7,14,21,30,35,42,49,56,
	57,50,43,36,29,22,15,23,30,37,44,51,58,59,52,45,38,31,39,46,53,60,61,54,47,55,62,63
    };


    /** JPEG Encoder */
    public static void encode(String fileName, String outFile, int Quality, boolean show, Log log)
    throws Exception {
	init();
	BufferedImage bi = BMP.read(new FileInputStream(fileName));
	long MILLIS = System.currentTimeMillis();
	HuffmanAC AC = new HuffmanAC();
	HuffmanDC DC = new HuffmanDC();
	int[][] Q = getQualityTab(Quality);
	int tmpr[] = null, tmpg[] = null, tmpb[] = null;
	float r = 0.0f, g = 0.0f, b = 0.0f;
	int lastr = 0, lastg = 0, lastb = 0;
	StringBuffer hufsb = new StringBuffer();
	int w = bi.getWidth()/8;
	int h = bi.getHeight()/8;
	long total = 0;
	
	int tmpi = 0, tmpj = 0;
	int tmpii = 0;
	int tmprgb = 0;
	int r2 = 0, g2 = 0, b2 = 0;
	long totalSize = 0;
	
	/** Raster Block 8x8 */
	RGB2YCrCb_2DCT8x8_Quanitized_ZigZag_HuffmanEncode:
	for (int i=0; i<w; i++)
	    for (int j=0; j<h; j++) {

		tmpr = new int[64];
		tmpg = new int[64];
		tmpb = new int[64];

		tmpi = i<<3;
		tmpj = j<<3;


		for (int ii=0; ii<8; ii++)
		for (int jj=0; jj<8; jj++) {
		    r = 0.0f; g = 0.0f; b = 0.0f;
		    tmpii = ii << 3;
		    for (int ii2=0; ii2<8; ii2++)
		    for (int jj2=0; jj2<8; jj2++) {
			tmprgb = bi.getRGB(tmpi+ii2, tmpj+jj2);
			r2 = getR(tmprgb); g2 = getG(tmprgb); b2 = getB(tmprgb);
			/** RGB to Y transform and multiplication the DCT coefficient matrix */
			r += getY(r2, g2, b2) * DCT[ii][jj][ii2][jj2];
			/** RGB to Cr transform and multiplication the DCT coefficient matrix */
			g += getCb(r2, g2, b2) * DCT[ii][jj][ii2][jj2];
			/** RGB to Cb transform and multiplication the DCT coefficient matrix */
			b += getCr(r2, g2, b2) * DCT[ii][jj][ii2][jj2];
		    }
		    
		    /** quanitized for Y */
		    tmpr[tmpii+jj] = (int)Math.round(r/(Q[0][tmpii+jj]));
		    /** quanitized for Cr */
		    tmpg[tmpii+jj] = (int)Math.round(g/(Q[1][tmpii+jj]));
		    /** quanitized for Cb */
		    tmpb[tmpii+jj] = (int)Math.round(b/(Q[1][tmpii+jj]));	    
		}

		/** Y Huffman Encoding */
		hufsb.append(huffmanEncode(tmpr, lastr, AC, DC));
		lastr = DC.lastDC;
		
		/** Cr Huffman Encoding */
		hufsb.append(huffmanEncode(tmpg, lastg, AC, DC));
		lastg = DC.lastDC;

		/** Cb Huffman Encoding */
		hufsb.append(huffmanEncode(tmpb, lastb, AC, DC));
		lastb = DC.lastDC;
		
		/** flush the huffman codes buffer if buffer length >= 4096 */
		if (hufsb.toString().length() >= 4096) {
		    totalSize += hufsb.toString().length();
		    /** write huffman string to binary file */
		    writeBlock(hufsb.toString(), Quality, w*8, h*8, outFile);
		    /** and new instance the String */
		    hufsb = new StringBuffer();
		}
	    }
	totalSize += hufsb.toString().length();
	writeBlock(hufsb.toString(), Quality, w*8, h*8, outFile);
	writeBlock(null, Quality, w*8, h*8, outFile);
	if (!show) return;

	/** Decompression arm for JPEG Encoder/Decoder */
	int ol = bi.getWidth()*bi.getHeight()*3;
	log.println("File: "+fileName+" "+bi.getWidth()+"x"+bi.getHeight()+" 24bits ");
	log.println("MUL Times:\t\t\t\t"+total);
	log.println("origin file size:\t\t"+ol+"bytes");
	log.println("JPEG compression size:\t\t"+(totalSize/8)+"bytes");

	double ratio = (ol/((float)totalSize/8));
	log.println("JPEG compression ratio:\t\t"+ratio);
	long ctime = System.currentTimeMillis()-MILLIS;
	log.println("JPEG compression time:\t\t"+ctime);
	MILLIS = System.currentTimeMillis();
	    String name = new File(fileName).getName();	
	final BufferedImage dc = Decoder.decode(outFile);
	final BufferedImage bi2 = bi;
	long dtime = (System.currentTimeMillis()-MILLIS);
	double RMSE = rmse(bi, dc);
	log.println("RMSE:\t\t\t\t"+RMSE);
	double BPP = (((float)totalSize)/(dc.getWidth()*dc.getHeight()*3));
	log.println("Bit Per Bits:\t\t\t"+BPP+" bits");
	log.println("JPEG decompression time:\t\t"+dtime+"ms\n");
    	final String title = "bpp:"+BPP+" rmse:"+RMSE+
			    " ratio:"+ratio+" origin_filesize:"+ol+"bytes compression_filesize:"+(totalSize/8)+
			    "bytes compression_time_millis:"+ctime+"ms decompression_time_millis:"+dtime+"ms";
	fos = null; lastByte = 0; index = 0; pow = 0;
	new Thread(new Runnable() {
	    public void run() {
		new ui.Show(title, dc);
	    }
	}).start();
    }

    static BufferedImage out;

    static double rmse(BufferedImage ori, BufferedImage c) {
	out = new BufferedImage(ori.getWidth(), ori.getHeight(), BufferedImage.TYPE_INT_RGB);
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
		out.setRGB(j*8+jj, i*8+ii, getRGB(r,g,b));
	    }
	    rmse2 += Math.sqrt(rmse/64);
	    times++;
	}
	return rmse2/times;
    }

    static double MSE(BufferedImage bi, BufferedImage out) {
	double rmse = 0.0;
	for (int i=0; i<bi.getWidth(); i++)
	for (int j=0; j<bi.getHeight(); j++)
	    rmse += Math.pow(getR(bi.getRGB(i, j)) - getR(out.getRGB(i, j)), 2);
	return rmse/bi.getWidth()*bi.getHeight();
    }

    static FileOutputStream fos;
    static byte lastByte = 0;
    static int index = 0;
    static int pow;
    

    /** 上次剩餘下來的位元: 將上次的位元合併到這次的位元 */
    static int lastByte(String s)
    throws IOException {
	int j = 0;
	for (int i=7-index; i>=0 && j<s.length(); i--, j++) {
	    pow = (int) Math.pow(2, i);
	    lastByte = (byte)(lastByte | (byte) (((s.charAt(j)-'0')<<(i))&pow));
	}
	
	if ((j+index)%8==0 && j != 0) {
	    fos.write(lastByte);
	    fos.flush();
	    lastByte = 0;
	}
	index = (j+index)%8;
	return j;
    }


    /** write bits to file (bits 以字串表示) */
    static void writeBlock(String huffman, int Quality, int w, int h, String outFile)
    throws IOException {
	/** initial the write stream */
	if (fos == null) {
	    fos = new FileOutputStream(new File(outFile));

	    /** the Header Section */
	    /** Quality parameter, Width parameter, Height parameter */
	    fos.write(intToBytes(Quality));
	    fos.write(intToBytes(w));
	    fos.write(intToBytes(h));
	    fos.flush();
	}
	
	/** close the write stream */
	if (huffman == null) {
	    
	    /** padding to 8 bits */
	    if (index != 0) {
		String padding = "";
		for (int i=0; i<8-index; i++)
		    padding +="0";
		lastByte(padding);
	    }
	    fos.close();
	    fos = null;
	    return;
	}

	/** 先寫入上次剩餘的位元 */
	int wx = lastByte(huffman);
	huffman = huffman.substring(wx, huffman.length());

	byte[] b = new byte[huffman.length()/8];
	for (int i=0; i<huffman.length()/8; i++) {
	    b[i] = (byte)((((huffman.charAt(i*8+0)-'0')<<7)&128) |
		    (((huffman.charAt(i*8+1)-'0')<<6)&64) |
		    (((huffman.charAt(i*8+2)-'0')<<5)&32) |
		    (((huffman.charAt(i*8+3)-'0')<<4)&16) |
		    (((huffman.charAt(i*8+4)-'0')<<3)&8) |
		    (((huffman.charAt(i*8+5)-'0')<<2)&4) |
		    (((huffman.charAt(i*8+6)-'0')<<1)&2) |
		    (((huffman.charAt(i*8+7)-'0'))&1));
	}
	/** 再寫入目前的位元 */
	fos.write(b);
	fos.flush();
	
	/** 將不滿足 8bits 對齊的位元 予以保存 等下次再寫入 */
	String lastStr = huffman.substring(huffman.length()-huffman.length()%8, huffman.length());
	lastByte(lastStr);
    }
   
    /** One-Integer to Four-Byte */
    public static byte[] intToBytes(int N) {
	byte[] b = new byte[4];
	b[0] = (byte) ((N >>> 24)&0xff);
	b[1] = (byte) ((N >>> 16)&0xff);
	b[2] = (byte) ((N >>> 8) & 0xff);
	b[3] = (byte) ((N)&0xff);
	return b;
    }

    /** Four-Byte to One-Integer */
    public static int bytesToInt(byte[] b) {
	int i = ((((int)b[0])&0xff) << 24) |
		((((int)b[1])&0xff) << 16) |
		((((int)b[2])&0xff) << 8) |
		(((int)b[3])&0xff);
	return i;
    }


    /** Huffman Encode */
    public static String huffmanEncode(int[] dct, int prevDC, HuffmanAC AC, HuffmanDC DC) {
	StringBuffer sb = new StringBuffer();
	String str = null;
	/** DC Huffman Encode */
	sb.append(str = DC.getHuffmanString(dct[0], prevDC));
	int runlength = 0;
	/** 剩餘的 AC 編碼: 1 ~ 63 */
	for (int i=1; i<64; i++) {
	    if (dct[ZZ[i]] == 0) {
		runlength++;
		continue;
	    }
	    /** 假如 RunLength 大於等於 16, 附加 ZRL(Zero Run Length) 上去 */
	    if (runlength >= 16) {
		for (int j=0; j<runlength/16; j++)
		    sb.append(AC.getHuffman()[15][0]);
		runlength = runlength%16;
	    }

	    /** 依照 runlength 的層及來將 AC 系數編碼,  */
	    sb.append(str = AC.getHuffmanString(dct[ZZ[i]], runlength));
	    /** 歸零 */
	    runlength = 0;
	}
	/** 最後附加 EOB(End of Block) */
	sb.append(AC.getHuffman()[0][0]);
	return sb.toString();
    }

    /** YCrCb to R */
    static int getR2(float Y, float Cb, float Cr) {
	int tmp = (int)Math.round(Y + 1.402*(Cr-128));
	if (tmp < 0) return 0;
	if (tmp > 255) return 255;
	return tmp;
    }

    /** YCrCb to G */
    static int getG2(float Y, float Cb, float Cr) {
	int tmp = (int)Math.round(Y - 0.34414*(Cb-128) - 0.71414*(Cr-128));
	if (tmp < 0) return 0;
	if (tmp > 255) return 255;
	return tmp;
    }

    /** YCrCb to B */
    static int getB2(float Y, float Cb, float Cr) {
	int tmp = (int)Math.round(Y + 1.772*(Cb-128));
	if (tmp < 0) return 0;
	if (tmp > 255) return 255;
	return tmp;
    }

    /** RGB to Y */
    static float getY(int r, int g, int b) {
	return (float)(0.299*r + 0.587*g + 0.114*b);
    }

    /** RGB to Cb */
    static float getCb(int r, int g, int b) {
	return (float)(-0.1687*r - 0.3313*g + 0.5*b + 128);    
    }

    /** RGB to Cr */
    static float getCr(int r, int g, int b) {
	return (float)(0.5*r - 0.4187*g - 0.0813*b + 128);    
    }

    /** cut Red from an integer */
    static int getR(int rgb) {
	return (int)((rgb>>>16)&0xff);
    }
    
    /** cut Green from an integer */
    static int getG(int rgb) {
	return (int)((rgb>>>8)&0xff);
    }
    
    /** cut Blue from an integer */
    static int getB(int rgb) {
	return (int)((rgb)&0xff);
    }

    /** emerge RGB to one Integer */
    static int getRGB(int r, int g, int b) {
	if (r < 0) r = 0;
	if (r > 255) r = 255;
	if (g < 0) g = 0;
	if (g > 255) g = 255;
	if (b < 0) b = 0;
	if (b > 255) b = 255;
	
	int rgb = (255&0xff)<<24 |
		(r&0xff)<<16 |
		(g&0xff)<<8 |
		(b&0xff);
	return rgb;
    }
}
