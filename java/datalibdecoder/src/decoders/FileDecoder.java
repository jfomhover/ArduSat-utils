package decoders;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;

import datalib.packets.Packet;
import datalib.syntax.SyntaxManager;
import datalib.writers.CSVWriter;
import datalib.writers.DataWriter;

public class FileDecoder {
	String filename;
	File file;
	FileInputStream fis;
	
	public void setFilename(String filename) {
		this.filename = filename;
	}
	
	public void open() {
		file = new File(filename);
		try {
			fis = new FileInputStream(file);
			Logger.getGlobal().log(Level.INFO, "*** opened file length="+fis.available());
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			fis = null;
		}
	}
	
	public void close() {
		if (fis == null)
			return;
		
		try {
			fis.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	public boolean available() {
		if (fis == null)
			return(false);
		try {
			return(fis.available() > 0);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return(false);
		}
	}
	
	public boolean decodePacket(DataWriter dw) {
		byte code[] = new byte[1];
		
		try {
			fis.read(code);
			Logger.getGlobal().log(Level.FINEST, "*** found packet code = "+(char)code[0]);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		Packet pckt = SyntaxManager.getPacket(code[0]);
		if (pckt == null) {
			Logger.getGlobal().log(Level.WARNING, "Packet type not found");
			return(false);
		}
		
		try {
			if (!pckt.decode(code[0], fis, dw)) {
				Logger.getGlobal().log(Level.WARNING, "failure to decode !");
				return(false);
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return(true);
	}
	
	
	public static void main(String[] args) {
		FileDecoder fd = new FileDecoder();
		
//		String filename = "testfiles/SPACEMA2.BIN";
		String filename = "testfiles/LUMCALIB.BIN";
		fd.setFilename(filename);
		fd.open();
		
		CSVWriter csvWriter = new CSVWriter();
		
		while(fd.available())
			fd.decodePacket(csvWriter);
	}

}
