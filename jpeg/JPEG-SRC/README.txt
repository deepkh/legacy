程式碼說明：

結構：
   img\BMP.java			BMP 讀取程式 -> 讀到 BufferedImage
   jpeg\Encoder.java 		JPEG Encoder 主要程式
   jpeg\Decoder.java		JPEG Decoder 主要程式
   jpeg\HuffmanAC.java		Huffman AC Table
   jpeg\HuffmanDC.java		Huffman DC Table
   ui\Viewer.java		程式進入點 (1): 
				這是一個簡單的 Encoder 與 Decoder
				可讀取 BMP 與 RAW (JPEG RAW)
				可依照壓縮比率寫入成 RAW (JPEG)
   ui\JPEGEmulation.java	程式進入點 (2):
				大概是展示用的程式, 壓縮後會有相關數據跑出來


by 黃冠達
deepkh@gmail.com
2005-11-27
致遠管理學院 資訊工程學系