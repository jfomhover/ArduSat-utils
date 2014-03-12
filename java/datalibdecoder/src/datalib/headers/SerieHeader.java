package datalib.headers;

import datalib.syntax.GenericUnit;
import datalib.util.Print;

/*
 *   char header;  // '!'
  uint8_t keyType:4;	// unit type of the key
  uint8_t keyDim:4;		// dimensionality of the key
  uint8_t valType:4;	// unit type of the value
  uint8_t valDim:4;		// dimensionality of the value
  uint16_t length;	// length of the serie array
 */
public class SerieHeader implements PacketHeaderInterface {
	public static final byte HEADERBYTE = '!';
	byte header;
	byte keyCode;
	byte keyType;
	byte keyDim;
	GenericUnit keyUnit;
	byte valCode;
	byte valType;
	byte valDim;
	GenericUnit valUnit;
	short count;
	
	@Override
	public int getHeaderLength() {
		return 5;
	}

	@Override
	public byte getHeaderCode() {
		return HEADERBYTE;
	}

	@Override
	public int getPacketLength() {
		int t_ret = 0;
		t_ret += keyUnit.getLength() * count;
		t_ret += valUnit.getLength() * count;
		t_ret += getHeaderLength();	// size of the header
		return(t_ret);
	}
	
	@Override
	public int getBodyLength() {
		return (getPacketLength() - getHeaderLength());
	}
	
	public byte getKeyCode() { return(keyCode); };
	public byte getValCode() { return(valCode); };
	public byte getKeyDim() { return(keyDim); };
	public byte getValDim() { return(valDim); };
	public byte getKeyType() { return(keyType); };
	public byte getValType() { return(valType); };
	public short getSerieCount() { return(count); };
	
	@Override
	public boolean decode(byte array[]) {
		if (array[0] != HEADERBYTE)
			return (false);
		
		header = array[0];
		keyCode = array[1];
		keyType = (byte) (array[1] & (byte)0x0F);
		keyDim = (byte) ((array[1] & 0xF0) >> 4);
		keyUnit = new GenericUnit();
		keyUnit.setCode(keyCode);

		valCode = array[2];
		valType = (byte) (array[2] & 0x0F);
		valDim = (byte) ((array[2] & 0xF0) >> 4);
		valUnit = new GenericUnit();
		valUnit.setCode(valCode);
		
		count = (short) (((short)array[3]) + ((short)((int)array[4]*(int)256)));
		return(true);
	}
	
	@Override
	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append(Print.printByteHex(header));
		sb.append(" ");
		sb.append(Print.printLByteHex(keyDim));
		sb.append(Print.printLByteHex(keyType));
		sb.append(" ");
		sb.append(Print.printLByteHex(valDim));
		sb.append(Print.printLByteHex(valType));
		sb.append(" ");
		sb.append(Print.printShortHex(count));
		return(sb.toString());
	}
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		SerieHeader hdr = new SerieHeader();
		byte test[] = { HEADERBYTE, 0x25, 0x42, 0x05, 0x01 };
		if (hdr.decode(test)) {
			System.out.println("decoding success!");
			System.out.println("hex = "+hdr.toString());
			System.out.println("packet length = "+hdr.getPacketLength());
		} else {
			System.out.println("decoding failed!");
		}
	}
}
