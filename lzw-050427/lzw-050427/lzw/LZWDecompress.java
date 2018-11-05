/*
 * �b 2005/4/20 �إ�
 *
 * TODO �p�G�n�ܧ�o�Ӳ��ͪ��ɮת��d���A�в���
 * ���� - �ߦn�]�w - Java - �{���X�˦� - �{���X�d��
 */
package lzw;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

/**
 * @author deepkh(���a�F)
 * @mail deepkh@ziwen.no-ip.biz
 * 
 * TODO �p�G�n�ܧ�o�Ӳ��ͪ����O���Ѫ��d���A�в��� ���� - �ߦn�]�w - Java - �{���X�˦� - �{���X�d��
 */
public class LZWDecompress {
	int[] prefix;
	int[] suffix;
	
	int codeUsed = 256;

	BufferedInputStream fin;

	BufferedOutputStream fout;

	public static void main(String[] args)
	throws Exception {
		long l = System.currentTimeMillis();
		LZWDecompress d = new LZWDecompress("SkypeSetup2.exe.lzw");
		d.decompress();
		System.out.println((System.currentTimeMillis()-l)/(double)1000);
	}
	
	LZWMonitor monitor;
	int reading, available;
	public LZWDecompress(String inFile) throws Exception {
		prefix = new int[4096];
		suffix = new int[4096];
		for (int i=0; i<256; i++) {
			prefix[i] = i;
			suffix[i] = i;
		}
		fin = new BufferedInputStream(new FileInputStream(new File(inFile)));
		fout = new BufferedOutputStream(new FileOutputStream(new File(inFile.substring(0,inFile.length()-4))));
		available = fin.available();
	}
	
	public LZWDecompress(String inFile, LZWMonitor monitor)
	throws Exception {
		this(inFile);
		this.monitor = monitor;
	}
	
	/**
	 * �r���ɮ榡
	 * Input		Output			Dict
	 * a			
	 * a			a				a,a 256
	 * b			a				a,b 257
	 * b			b				b,b 258
	 * 257			b				b,257 259
	 * 260			ab				257,260 260	    -> suffix replace by prefix first byte
	 * 257			aba				260,257 261
	 * 259			ab				257,259 262
	 * 258			ba				259,258 263
	 * a			bb				258,a	264
	 * 			a
	 */

	/**
	 * �ѽX���S�ʡA�Ĥ@�� 12bits �nø�X�ҥN����Ʃ�r�媺�Ҧ��줸��
	 * 			  �ĤG�� 12bits �uø�X�ҥN����Ʃ�r�媺�����줸��
	 * @throws IOException
	 */
	public void decompress()
	throws IOException {
		int first = get12Bits();
		int second = 0;
		while((second = get12Bits()) != -1) {
			/** ��X�Ĥ@�� 12bits */
			writePair(first);
			/** �سy�r���ɡA�S���j�� 2^12 (4096)
			 * �r���ɮ榡���G�� 12bits ��ơA�Ĥ@��
			 * �O�r���]�N�O first�A�ĤG�ӬO�r�� second
			 */
			if (codeUsed < 4096) {
				/** �r�� */
				prefix[codeUsed] = first;
				/** �r�����G�ر��p�A�@�ئr���O�٨S���سy���r���ɡA����
				 * �N�� first ���Ĥ@�� 12bits�A�t�@�جO�w�s�b���r����
				 * �N���������@�� 12bits ��ơA
				 * (�]���@�Ӧr����id�N�O�@�դw�s�r���ɪ��@��줸��)
				 */
				if (second == codeUsed)
					suffix[codeUsed] = getD(first);
				else
					suffix[codeUsed] = getD(second);
				codeUsed++;
			}
			/*
			 * �m��
			 */
			first = second;
		}
		writePair(first);
		fout.flush();
		fin.close();
		fout.close();
	}
	

	void writePair(int i)
	throws IOException {
		/** ���j(���|)�k */
		if (i < 256) {
			fout.write(i);
			return;
		}
		if (prefix[i] > 255)
			writePair(prefix[i]);
		else
			fout.write(prefix[i]);
		fout.write(suffix[i]);
		
		/** �j��k */
//		int index = 0;
//		int[] tmp = new int[4096];
//		while(true) {
//			tmp[index++] = suffix[i];
//			if ((i=prefix[i]) < 256)
//				break;
//		}
//		fout.write(i);
//		for (int z=(index-1);z>=0;z--)
//			fout.write(tmp[z]);
	}
	

	int getD(int i) {
		while((i = prefix[i]) > 255) {
			;
		}
		return i;
	}
	
	int r = -1;
	boolean hasLeftByte = false;
	int lastByte = 0;
	int temp = 0;
	int print = 0;
	int t = 0;

	int count = 0;
	public int get12Bits()
	throws IOException {
		r = fin.read();
		reading++;
		if (r == -1) return -1;
		if (lastByte == 0) {
			temp = r << 4;
			lastByte = 4;
			return get12Bits();
		} else if (lastByte == 4) {
			print = temp + (r >> 4);
			lastByte = 8;
			temp = r << 8;
			temp &= 4095;
		} else if (lastByte == 8) {
			print = temp + r;
			lastByte = 0;
			if (monitor != null && (count++%50 == 0))
				monitor.updateProgress(reading, available);
		}

		/** [Debug] print every 12bits form file stream*/
		/*for (int i=11; i>=0; i--)
		    System.out.print((print>>i)&1);
		System.out.println(" "+print+"  \t"+(char)print); */
		return print;
	}
}

