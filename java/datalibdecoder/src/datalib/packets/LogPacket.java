package datalib.packets;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;

import datalib.headers.LogHeader;
import datalib.util.Print;
import datalib.writers.CSVWriter;
import datalib.writers.DataWriter;

public class LogPacket extends Packet {
	byte content[] = null;

	public LogPacket() {
		super();
		setPacketHeader(new LogHeader());
	}

	@Override
	public boolean decode(byte code, InputStream istr, DataWriter dw) throws IOException {
		if (!decodeHeader(code, istr)) {
			System.out.println("Failed to decode header");
			return(false);
		}
			
		int t_len = hdr.getPacketLength() - hdr.getHeaderLength();
		content = new byte[t_len];
		int t_ret = 0;
		System.out.println("Reading content size "+t_len+" plen="+hdr.getPacketLength()+" hlen="+hdr.getHeaderLength());
		
		if (t_len > 0)
			t_ret = istr.read(content, 0, t_len);

		if (t_ret < t_len) {
			System.out.println("Not enough data");
			return(false);
		}

		if (dw != null)
			dw.onLogString(Print.printArrayAsSTR(content));

		return(true);
	}

	public String toString() {
		return(Print.printArrayAsSTR(content));
	}

	public static void main(String[] args) {
		byte array[] = new byte[] { 'L', 5, 'S', 'T', 'O', 'P', '!' };
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

		LogPacket pckt = new LogPacket();
		
		try {
			if (!pckt.decode(code[0], istr, new CSVWriter()))
				System.out.println("failure to decode !");
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
//		System.out.println(pckt.toString());
	}
}
