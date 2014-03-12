package datalib.headers;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.List;

import datalib.syntax.DataType;
import datalib.syntax.SyntaxManager;
import datalib.util.Print;

public class ChunkHeader implements PacketHeaderInterface {
	public static final byte HEADERBYTE = '#';	
	byte header;
	short datatypes;
	
	@Override
	public int getHeaderLength() {
		// TODO Auto-generated method stub
		return 3;
	}
	
	@Override
	public int getBodyLength() {
		return (getPacketLength() - getHeaderLength());
	}
	

	@Override
	public byte getHeaderCode() {
		// TODO Auto-generated method stub
		return HEADERBYTE;
	}
	
	public short getDataTypes() { return(datatypes); };

	@Override
	public int getPacketLength() {
		// TODO Auto-generated method stub
		List<DataType> types = SyntaxManager.searchTypes(datatypes);
		int t_len = getHeaderLength();
		for (int i=0; i<types.size(); i++)
			t_len += types.get(i).getDataSize();
		return t_len;
	}
	
	@Override
	public boolean decode(byte array[]) {
		if (array[0] != HEADERBYTE)
			return(false);
		header = array[0];
		//datatypes = (short) (array[1] + array[2] << 8);
		byte t_array[] = new byte[4];
		t_array[0] = array[1];
		t_array[1] = array[2];
		t_array[2] = 0x00;
		t_array[3] = 0x00;
		
		ByteBuffer bb = ByteBuffer.wrap(t_array);
		bb.order(ByteOrder.LITTLE_ENDIAN);
		datatypes = bb.getShort();		
//		datatypes = array[i] + array[2] * 256;
//		datatypes = (short) (((short)array[1]) + ((short)((byte)array[2]*(int)256)));
		return(true);
	}
	
	@Override
	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append(Print.printByteHex(header));
		sb.append(" ");
		sb.append(Print.printShortHex(datatypes));
		return(sb.toString());
	}
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		ChunkHeader hdr = new ChunkHeader();
		byte test[] = { '#', (byte) 0b11110001, 0x00 };
		if (hdr.decode(test)) {
			System.out.println("decoding success!");
			System.out.println(hdr.toString());
			System.out.println(hdr.getPacketLength());
		} else {
			System.out.println("decoding failed!");
		}
	}
}
