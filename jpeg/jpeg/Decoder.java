/** JPEG Decoder implements by deepkh@gmail.com 2006-11-27 */
package jpeg;
import javax.imageio.plugins.jpeg.JPEGQTable;
import javax.imageio.plugins.jpeg.JPEGHuffmanTable;
import java.io.*;
import java.awt.image.*;
import img.*;
import javax.swing.*;
import java.awt.*;

public class Decoder {

    public static void main(String[] a)
    throws Exception {
	decode(a[0]);
    }

    /** JPEG Decoder Function */
    public static BufferedImage decode(String fileName)
    throws Exception {
	Encoder.init();
	HuffmanAC AC = new HuffmanAC();
	HuffmanDC DC = new HuffmanDC();
    
	/** read jpeg compressed file */
	FileInputStream fis = new FileInputStream(fileName);
	/** info[0]=quailty level, info[1]=width, info[2]=height */
	int info[] = readHeader(fis);
	/** huffman bits stream */
	String huf = readBlock(fis);
	int[][] Q = Encoder.getQualityTab(info[0]);
	
	BufferedImage out = new BufferedImage(info[1], info[2], BufferedImage.TYPE_INT_RGB);
	int tmpr[] = null, tmpg[] = null, tmpb[] = null;
	float ir = 0.0f, ig = 0.0f, ib = 0.0f;
	int lastr = 0, lastg = 0, lastb = 0;
	
	int tmpi = 0, tmpj = 0;
	int tmpii2 = 0;
	HuffmanDecode_iZigZag_iQuanitized_2IDCT8x8_YCrCb2RGB:
	for (int i=0; i<out.getWidth()/8; i++) {
	    for (int j=0; j<out.getHeight()/8; j++) {

		/** Y Huffman Deocde. �åB�� ZigZag Scan �٭즨 2 ����ư}�C */
		tmpr = huffmanDecode(huf, lastr, AC, DC);   
		huf = huf.substring(reads, huf.length());
		lastr = tmpr[0];
		
		/** Cr Huffman Deocde. �åB�� ZigZag Scan �٭즨 2 ����ư}�C */
		tmpg = huffmanDecode(huf, lastg, AC, DC);   
		huf = huf.substring(reads, huf.length());
		lastg = tmpg[0];

		/** Cb Huffman Deocde. �åB�� ZigZag Scan �٭즨 2 ����ư}�C */
		tmpb = huffmanDecode(huf, lastb, AC, DC);  
		huf = huf.substring(reads, huf.length());
		lastb = tmpb[0];

		
		tmpi = i << 3;	/** tmpi = i * 8 */
		tmpj = j << 3;  /** tmpj = j * 8 */
		for (int ii=0; ii<8; ii++)
		for (int jj=0; jj<8; jj++) {
		    ir = 0.0f; ig = 0.0f; ib = 0.0f;
		    for (int ii2=0; ii2<8; ii2++) {
			tmpii2 = ii2 << 3;	//  ii2*8
			for (int jj2=0; jj2<8; jj2++) {
			    /** �N Y ���W�G�רt�� Q[0], �A Inverse-DCT �ഫ */
			    ir += Math.round(tmpr[tmpii2+jj2]*Q[0][tmpii2+jj2]) * Encoder.DCT[ii2][jj2][ii][jj];
			    /** �N Cr ���W�G�רt�� Q[0], �A Inverse-DCT �ഫ */
			    ig += Math.round(tmpg[tmpii2+jj2]*Q[1][tmpii2+jj2]) * Encoder.DCT[ii2][jj2][ii][jj];
			    /** �N Cb ���W�G�רt�� Q[0], �A Inverse-DCT �ഫ */
			    ib += Math.round(tmpb[tmpii2+jj2]*Q[1][tmpii2+jj2]) * Encoder.DCT[ii2][jj2][ii][jj];
			}
		    }
		    /** final stage: YCrCb to RGB */
		    out.setRGB(tmpi+ii, tmpj+jj, Encoder.getRGB(Encoder.getR2(ir,ig,ib), Encoder.getG2(ir,ig,ib), Encoder.getB2(ir,ig,ib)));
		}
	    }
	}
	return out;
    }
   
    
    static int reads = 0;
    /** Huffman Decode */
    /** ������ɮצ�yŪ��, ���B�z DC, �A�B�z AC, ����Ū���� EOB(1010) �h���� */
    static int[] huffmanDecode(String huf, int prevDC, HuffmanAC AC, HuffmanDC DC) {
	int index = 0, len = 0;
	int tmp = 0;
	int d=1;
    	int[] tmprr = new int[64];
	reads = 0;
	int tmpz = 0;

	/** DC Decode */
    	DC:
    	for (; d<huf.length(); d++) {
    	    len = huf.substring(index, d).length()+1;
    	    for (int ii=0; ii<DC.HUFFMAN.length; ii++)
    		if (DC.HUFFMAN[ii].equals(huf.substring(index, index+len))) {
		    /** �p�G DC �� SSSS �� 00, ���N���ΥhŪ���Ӽƭ�  */
		    if (huf.substring(index, index+len).equals("00")) {
			tmprr[0] = 0+prevDC;
			reads += 2;
		    } else {
			/** �p�G DC �� SSSS ���� 00, �����hŪ���ӤG�i��ƭ� */
			tmp = getBin(huf.substring(d+1, d+1+ii));
			tmpz = tmp;
			reads += huf.substring(index, index+len).length() + huf.substring(d+1, d+1+ii).length();
			tmprr[0] = tmp+prevDC;
		    }
		    index = d + ii + 1;
		    d = index+1;
		    break DC;
	    	}
    	}
		
	/** AC Decode: 1 ~ 63 */
    	int ZRL = 1;
    	AC:
    	for (; d<huf.length(); d++) {
    	    len = huf.substring(index, d).length()+1;
    	    SUB:
    	    for (int ii=0; ii<AC.HUFFMAN.length; ii++)
    	    for (int jj=0; jj<AC.HUFFMAN[ii].length; jj++) {
    		if (AC.HUFFMAN[ii][jj].length() == len)
    		if (AC.HUFFMAN[ii][jj].equals(huf.substring(index, index+len))) {
		    
		    /** EOB(End Of Block) 1010 */
		    if (ii == 00 && jj == 00) {
			reads += huf.substring(index, index+len).length();
	    		break AC;
    		    }

		    /** ZRL(Zero Run Length) 1111 1111 001 */
	    	    if (ii == 15 && jj == 0) {
	    		index = d + 1;
	    		d = index;
	    		ZRL += 16;
			reads += huf.substring(index, index+len).length();
	    		break SUB;
	    	    }

		    ZRL += ii;
		    reads += huf.substring(index, index+len).length()+huf.substring(d+1, d+1+jj).length();
    		    tmp = getBin(huf.substring(d+1, d+1+jj));
		    tmprr[Encoder.ZZ[ZRL]] = tmp;
	    	    ZRL += 1;
		    index = d + jj + 1;
		    d = index;
		    break SUB;
	    	}
    	    }
    	}
	return tmprr;
    }

    /** �N�r��Ҫ�ܪ��줸�ର integer */
    static int getBin(String str) {
	int tmp = 0;
	boolean negative = (str.charAt(0) == '0'?true:false);
	for (int i=str.length()-1, j=0; i>=0; i--, j++) { 
	    if (negative)
		tmp = tmp | (((str.charAt(j)-'0')+1)&1) << i;
	    else
		tmp = tmp | ((str.charAt(j)-'0')&1) << i;
	}
	if (negative)
	    tmp = 0 - tmp;
	return tmp;
    }

    /** Ū�����Y: Quality, Width, Height */
    static int[] readHeader(FileInputStream fis)
    throws IOException {
	byte[] Q = new byte[4];
	byte[] w = new byte[4];
	byte[] h = new byte[4];
	fis.read(Q);
	fis.read(w);
	fis.read(h);
	return new int[] {
	    Encoder.bytesToInt(Q), 
	    Encoder.bytesToInt(w), 
	    Encoder.bytesToInt(h)
	};
    }

    /** �N bits stream �ର string */
    static String readBlock(FileInputStream fis)
    throws IOException {
	StringBuffer sb = new StringBuffer();
	int rd = 0;
	byte[] buf = new byte[10240];
	while((rd = fis.read(buf)) != -1) {
	    for (int i=0; i<rd; i++) {
		sb.append((char)(((buf[i]>>>7)&1)+'0'));
		sb.append((char)(((buf[i]>>>6)&1)+'0'));
		sb.append((char)(((buf[i]>>>5)&1)+'0'));
		sb.append((char)(((buf[i]>>>4)&1)+'0'));
		sb.append((char)(((buf[i]>>>3)&1)+'0'));
		sb.append((char)(((buf[i]>>>2)&1)+'0'));
		sb.append((char)(((buf[i]>>>1)&1)+'0'));
		sb.append((char)(((buf[i]>>>0)&1)+'0'));
	    }
	}
	fis.close();
	return sb.toString();
    }
}

