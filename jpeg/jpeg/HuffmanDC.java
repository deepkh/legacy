/** JPEG Huffman DC Code Book implements by deepkh@gmail.com 2006-11-27 */

package jpeg;


public class HuffmanDC  {

    static int[][] CATEGORY = new int[][] {
	{0, 0},		    //0
	{-1,1},		    //1
	{-3,3},		    //2
	{-7,7},		    //3
	{-15,15},	    //4
	{-31,31},	    //5
	{-63,63},	    //6
	{-127,127},	    //7
	{-255,255},	    //8
	{-511,511},	    //9
	{-1023,1023},	    //10
	{-2047,2047},	    //11
	{-4095,4095}	    //12
    };

    /** Luminance DC Huffman */
    public static String[] HUFFMAN = new String[] {
	"00",		    //0
	"010",		    //1
	"011",		    //2
	"100",		    //3
	"101",		    //4
	"110",		    //5
	"1110",		    //6
	"11110",	    //7
	"111110",	    //8
	"11111110",	    //9
	"111111110",	    //10
	"1111111110",	    //11
    };

    public String[] getHuffman() {
	return HUFFMAN;
    }

    public int[][] getCategory() {
	return CATEGORY;
    }

    public static int lastDC;

    public String getHuffmanString(int DC, int prevDC) {

	int DIFF = DC - prevDC;
	int size = getSize(DIFF);
	if (DIFF == 0) {
	    lastDC = DC;
	    return "00";
	}
//	System.out.println("("+size+")"+DIFF);
	String hufstr = getHuffman()[size] +
			getBinaryString(DIFF, size);
	lastDC = DC;
//	System.out.println(hufstr);

	return hufstr;
    }

    public String getBinaryString(int code, int len) {
	int tmp = code & (int)(Math.pow(2, len)-1);
	int tmp2 = Math.abs(code);
	StringBuffer str = new StringBuffer();
	for (int i=(len-1); i>=0; i--) {
	    if (code > 0) {
		str.append(((tmp>>>i)&1));
	    } else {
		str.append((((tmp2>>>i)&1)+1)&1);
	    }
	}
	return str.toString();
    }

    public int getSize(int code) {
	for (int i=0; i<CATEGORY.length; i++)
	    if (code >= CATEGORY[i][0] && code <= CATEGORY[i][1])
		return i;
	return -1;
    }
}
