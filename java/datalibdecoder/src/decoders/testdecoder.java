package decoders;

import java.io.ByteArrayInputStream;
import java.io.IOException;

import datalib.packets.Packet;
import datalib.syntax.SyntaxManager;
import datalib.writers.CSVWriter;

public class testdecoder {
	// Output taken from the Data_buildchunk.ino example sketch
	public static byte chunkpacket[] = {
			0x23, 0x31, 0x00, (byte) 0xF2, 0x07, 0x00, 0x00, 0x19, 0x00, 0x40, 0x01
	};


	// Output taken from the Data_userdefinedblock.ino
	public static byte chunkudblockpacket[] = {
			0x23, 0x01, 0x60, (byte) 0xE9, 0x03, 0x00, 0x00, 0x00,
			0x04, 0x08, 0x15, 0x16, 0x0D, 0x53, 0x54, 0x4F,
			0x50
	};

	// Output taken from the EMUSD_send_chunk.ino
	public static byte chunktmppacket[] = {
			0x23, (byte) 0xF1, 0x00, (byte) 0xA3, 0x15, 0x00, 0x00, 0x04,
			0x00, 0x08, 0x00, 0x0F, 0x00, 0x10, 0x00 
	};

	// Output of the Data_userpacket.ino example sketch
	public static byte userpacket[] = {
		0x55, 0x19, 0x0F, 0x56, 0x0E, 0x49, 0x40, 0x05,
		0x2A, 0x00, 0x07, 0x47, (byte) 0xBC, (byte) 0xDE, 0x02, 0x03,
		(byte) 0xAB, (byte) 0xCD, (byte) 0xEF, 0x01, 0x0D, 0x53, 0x54, 0x4F,
		0x50 
	};

	// Output of the Data_logpacket.ino example sketch
	public static byte logpacket[] = {
			'L', 0x13, 0x6C, 0x6F, 0x67, 0x20, 0x61, 0x6E,
			0x79, 0x74, 0x68, 0x69, 0x6E, 0x67, 0x20, 0x68,
			0x65, 0x72, 0x65
	};

	public static void decodeBytePacket(byte array[]) {
		ByteArrayInputStream istr = new ByteArrayInputStream(array);
		
		byte code[] = new byte[1];
		istr.reset();
		try {
			istr.read(code);
			System.out.println("code = "+(char)code[0]);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		Packet pckt = SyntaxManager.getPacket(code[0]);
		if (pckt == null) {
			System.out.println("Packet type not found");
			return;
		}
		
		try {
			if (!pckt.decode(code[0], istr, new CSVWriter()))
				System.out.println("failure to decode !");
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	};
	
	public static void main(String[] args) {
		System.out.println("*** logpacket:");
		decodeBytePacket(logpacket);
		System.out.println("*** chunkpacket:");
		decodeBytePacket(chunkpacket);
		System.out.println("*** userpacket:");
		decodeBytePacket(userpacket);
//		System.out.println("*** chunktmppacket:");
//		decodeBytePacket(chunktmppacket);
	}

}
