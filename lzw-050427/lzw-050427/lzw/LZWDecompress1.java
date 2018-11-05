/*
 * 在 2005/4/20 建立
 *
 * TODO 如果要變更這個產生的檔案的範本，請移至
 * 視窗 - 喜好設定 - Java - 程式碼樣式 - 程式碼範本
 */
package lzw;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Hashtable;

/**
 * @author Administrator
 * 
 * TODO 如果要變更這個產生的類別註解的範本，請移至 視窗 - 喜好設定 - Java - 程式碼樣式 - 程式碼範本
 */
public class LZWDecompress1 {

	Hashtable map,map1;

	int codeUsed = 256;

	FileInputStream fin;

	FileOutputStream fout;

	public static void main(String[] args)
	throws Exception {
		long l = System.currentTimeMillis();
		LZWDecompress1 d = new LZWDecompress1("SkypeSetup2.exe.lzw");
		d.decompress();
		System.out.println((System.currentTimeMillis()-l)/(double)1000);
	}

	public LZWDecompress1(String inFile) throws Exception {
		map = new Hashtable(4095);
		map1 = new Hashtable(4095);
		fin = new FileInputStream(new File(inFile));
		fout = new FileOutputStream(new File(inFile + ".exe"));
	}

	public void decompress()
	throws IOException {
		String a="",b="";
		StringBuffer sb = new StringBuffer();
		int first = get12Bits();
		System.out.println((char )first);
		int second = 0;
		while((second = get12Bits()) != -1) {
			a = String.valueOf((char)first);
			b = String.valueOf((char)second);
			if (first > 255)
				a = (String) map1.get(new Integer(first));
			if (second > 255) {
				b = (String) map1.get(new Integer(second));
				if (b == null)
					b = a+a.charAt(0);
			}
			//if (map.get(a+b) == null) {
				//writeOut(a);
				//sb.append(a);
				fout.write(a.getBytes());
				if (codeUsed < 4096) {
					if (second < 256) {
						map1.put(new Integer(codeUsed),a+b);
						map.put(a+b,new Integer(codeUsed));
						//System.out.println(b+"\t"+a+"\t"+a+b+" "+codeUsed);
					} else {
						//System.out.println(b+"\t"+a+"\t"+a+String.valueOf(b.charAt(0))+" "+codeUsed);
						map1.put(new Integer(codeUsed), a+String.valueOf(b.charAt(0)));
						map.put(a+String.valueOf(b.charAt(0)),new Integer(codeUsed));
					}
					codeUsed++;
				}
				first = second;
			//} else {
				//writeOut(a);
				//System.out.println("fuck");
				//first = second;
			//}
		}
		//sb.append(b);
		//fout.write(sb.toString().getBytes());
		fout.write(b.getBytes());
		fout.flush();
		fin.close();
		fout.close();
		System.out.println("\n"+sb);
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
