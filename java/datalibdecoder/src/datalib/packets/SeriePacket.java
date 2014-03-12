package datalib.packets;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;

import datalib.headers.SerieHeader;
import datalib.syntax.GenericUnit;
import datalib.writers.CSVWriter;
import datalib.writers.DataWriter;

public class SeriePacket extends Packet {

	public SeriePacket() {
		super();
		setPacketHeader(new SerieHeader());
	}

	@Override
	public boolean decode(byte code, InputStream istr, DataWriter dw) throws IOException {
		if (!decodeHeader(code, istr))
			return(false);
		
		SerieHeader s_hdr = (SerieHeader)hdr;

		GenericUnit k_Unit = new GenericUnit();
		k_Unit.setCode(s_hdr.getKeyCode());
		GenericUnit v_Unit = new GenericUnit();
		v_Unit.setCode(s_hdr.getValCode());


		for (int i=0; i<s_hdr.getSerieCount(); i++) {
			byte k_array[] = new byte[k_Unit.getLength()];
			byte v_array[] = new byte[v_Unit.getLength()];
			
			istr.read(k_array);
			istr.read(v_array);
			
			k_Unit.decode(s_hdr.getKeyCode(), k_array, 0);
			v_Unit.decode(s_hdr.getValCode(), v_array, 0);

			dw.openSerieEntry(s_hdr.getKeyType(), s_hdr.getKeyDim(), s_hdr.getValType(), s_hdr.getValDim());
			
			for (int j=0; j<s_hdr.getKeyDim(); j++)
				dw.onSerieData(k_Unit.getValue(j));
			for (int j=0; j<s_hdr.getValDim(); j++)
				dw.onSerieData(v_Unit.getValue(j));

			dw.closeSerieEntry();
		}
		return(true);
	}

	public static void main(String[] args) {
		byte array[] = new byte[] { '#', 0x01, 0x00, 0x01, 0x00, 0x00, 0x00 };
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

		SeriePacket pckt = new SeriePacket();
		
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
