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
import java.util.Hashtable;

/**
 * @author Administrator
 * 
 * TODO �p�G�n�ܧ�o�Ӳ��ͪ����O���Ѫ��d���A�в��� ���� - �ߦn�]�w - Java - �{���X�˦� - �{���X�d��
 */
public class LZWDecompress2 {

	Hashtable map,map1;

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

	public LZWDecompress2(String inFile) throws Exception {
		map = new Hashtable(4095);
		map1 = new Hashtable(4095);
		fin = new BufferedInputStream(new FileInputStream(new File(inFile)));
		fout = new BufferedOutputStream(new FileOutputStream(new File(inFile + ".exe")));
	}
	
	class ByteAry {
		byte[] b;
		public ByteAry(int i) {
			b = new byte[1];
			b[0] = (byte) i;
		}
		
		public ByteAry(byte[] b1, byte[] b2) {
			this.b = new byte[b1.length+b2.length];
			int i=0;
			for (; i<b1.length; i++)
				this.b[i] = b1[i];
			for (int j=0; j<b2.length; j++,i++)
				this.b[i] = b2[j]; 
		}
		
		public byte[] getBytes() {
			return b;
		}
	}

	public void decompress()
	throws IOException {
		ByteAry a=null,b=null;
		StringBuffer sb = new StringBuffer();
		int first = get12Bits();
		System.out.println((char )first);
		int second = 0;
		while((second = get12Bits()) != -1) {
			a = new ByteAry(first);
			b = new ByteAry(second);
			if (first > 255)
				a = (ByteAry) map1.get(new Integer(first));
			if (second > 255) {
				b = (ByteAry) map1.get(new Integer(second));
				if (b == null)
					b = new ByteAry(a.getBytes(),new byte[]{a.getBytes()[0]});
			}

			fout.write(a.getBytes());
			if (codeUsed < 4096) {
				if (second < 256) {
					map1.put(new Integer(codeUsed), new ByteAry(a.getBytes(),b.getBytes()));
				} else {
					map1.put(new Integer(codeUsed),new ByteAry(a.getBytes(),new byte[]{b.getBytes()[0]}));
				}
				codeUsed++;
			}
			first = second;
		}

		fout.write(b.getBytes());
		fout.flush();
		fin.close();
		fout.close();
	}
	
	void writeOut(String a) {
		System.out.print(a);
	}

	int r = -1;
	boolean hasLeftByte = false;
	int lastByte = 0;
	int temp = 0;
	int print = 0;
	int t = 0;

	public int get12Bits()
	throws IOException {
		r = fin.read();
		if (r == -1) return -1;
		if (lastByte == 0) {
			temp = r << 4;
			lastByte = 4;
			return get12Bits();
		} else if (lastByte == 4) {
			print = temp + (r >> 4);
			//print = r >> 4;
			lastByte = 8;
			temp = r << 8;
			temp &= 4095;
		} else if (lastByte == 8) {
			print = temp + r;
			lastByte = 0;
		}
//      	for (int i=11; i>=0; i--)
//      		System.out.print((print>>i)&1);
//      	System.out.println(" "+print+"  \t"+(char)print);
		return print;
	}
}
