/*
 * 在 2005/4/20 建立
 *
 * TODO 如果要變更這個產生的檔案的範本，請移至
 * 視窗 - 喜好設定 - Java - 程式碼樣式 - 程式碼範本
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
 * @author deepkh(黃冠達)
 * @mail deepkh@ziwen.no-ip.biz
 *
 * TODO 如果要變更這個產生的類別註解的範本，請移至
 * 視窗 - 喜好設定 - Java - 程式碼樣式 - 程式碼範本
 */
public class LZWCompress {
	
	Hashtable map,map1;
	int codeUsed = 256;
	BufferedInputStream fin;
	BufferedOutputStream fout;
	int leftByte;
	boolean isLeftByte;

	public static void main(String[] args)
	throws Exception {
		long l = System.currentTimeMillis();
		LZWCompress c = new LZWCompress("SkypeSetup2.exe");
		c.compress();
		System.out.println((System.currentTimeMillis()-l)/(double)1000);
		
		/** [Debug] print every 12bits form file stream*/
//	     FileInputStream fin = new FileInputStream(new File("t1.txt.lzw"));
//	      int r = -1;
//	      boolean hasLeftByte = false;
//	      int lastByte =0;
//	      int temp = 0;
//	      int print = 0;
//	      int t = 0;
//	      while((r = fin.read())!=-1) {
//	      	if (lastByte == 0) {
//	      		temp = r << 4;
//	      		lastByte = 4;
//	      		continue;
//	      	} else if (lastByte == 4) {
//	      		print = temp + (r >> 4);
//	      		//print = r >> 4;
//	      		lastByte = 8;
//	      		temp = r << 8;
//	      		temp &= 4095;
//	      	} else if (lastByte == 8) {
//	      		print = temp + r;
//	      		lastByte = 0;
//	      	}
//	      	for (int i=11; i>=0; i--)
//	      		System.out.print((print>>i)&1);
//	      	System.out.println(" "+print+"  \t"+(char)print);
//	      	if (t++ == 20)
//	      		break;
//	      }
//	      System.out.println();
//	      fin.close();
//	      fin = new FileInputStream(new File("SSQ"));
//	      while((r = fin.read()) != -1)
//	      	for (int i=7; i>=0; i--)
//	      		System.out.print(((r>>i)&1)+(i==0?"\n":""));
	}
	
	LZWMonitor monitor;
	
	public LZWCompress(String inFile)
	throws Exception {
		map = new Hashtable(4095);
		map1 = new Hashtable(4095);
		fin = new BufferedInputStream(new FileInputStream(new File(inFile)));
		fout = new BufferedOutputStream(new FileOutputStream(new File(inFile+".lzw")));
		available = fin.available();
	}
	
	public LZWCompress(String inFile, LZWMonitor monitor)
	throws Exception {
		this(inFile);
		this.monitor = monitor;
	}
	
	/**
	 * 字典檔格式
	 * Input			Output			Dict
	 * a
	 * a				a				a,a	256
	 * b				a				a,b 257
	 * b				b				b,b	258
	 * a				b				b,a 259
	 * b				
	 * a				257				257,a 260
	 * b				
	 * a				
	 * a				260				260,a 261
	 * b				
	 * b				257				257,b 262
	 * a				
	 * b				259				259,b 263
	 * b				
	 * a				258				258,a 264
	 * 					a
	 * @throws IOException
	 */
	
	public void compress()
	throws IOException {
		int first = fin.read();
		System.out.println(((char)first));
		reading++;
		int second = 0;
		Integer rs = null;
		while((second = fin.read()) != -1) {
			reading++;
			if ((rs = (Integer) map.get(new Integer((first << 8)+second))) != null) {
				first = rs.intValue();
			} else {
				writeOut(first);
				if (codeUsed < 4096) {
					map.put(new Integer((first << 8)+second), new Integer(codeUsed++));
					if (first < 256) {
						map1.put(new Integer(codeUsed-1),String.valueOf((char)first)+String.valueOf((char)second));
					} else {
						map1.put(new Integer(codeUsed-1),((String)map1.get(new Integer(first)))+String.valueOf((char)second));
					}
				}
				char c1=0,c2=0;
				c1 = (char) first;
				c2 = (char) second;
				first = second;
			}
		}
		
		writeOut(first);
		if (isLeftByte) {
			fout.write(leftByte);
			fout.flush();
		}
		fin.close();
		fout.close();
	}
	
	int available;
	int reading;
	/**
	 * 寫入 12bits , 
	 * 		if isLeftByte = false then
	 * 		寫入 11 ~ 4 bits
	 * 			11 10 9 8 7 6 5 4
	 * 		存入剩餘 3 2 1 0 bits
	 * 	 	LeftByte = 3 2 1 0 0000
	 * 	
	 * 		if isLeftByte = true then
	 * 		寫入 LeftByte + val >> 8
	 * 			3210 11 10 9 8
	 * 		寫入 val & 0xff
	 * 			765423210
	 * @param val
	 * @throws IOException
	 */
	int count = 0;
	public void writeOut(int val)
	throws IOException {
		if (!isLeftByte) {
			leftByte = ((val&0xf) << 4);
			fout.write((val>>4));
			isLeftByte = true;

		} else {
			leftByte += (val >> 8);
			fout.write(leftByte);
			fout.write((val&0xff));
			fout.flush();
			isLeftByte = false;
			if (monitor != null && (count++%50 == 0))
				monitor.updateProgress(reading, available);
		}

	}
}
