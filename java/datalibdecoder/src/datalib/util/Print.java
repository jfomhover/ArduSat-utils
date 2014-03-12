package datalib.util;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.logging.Level;
import java.util.logging.Logger;

public class Print {
	public static final char DEFAULT_SEPARATOR = ';';
	
	public static String printArrayAsSTR(byte value[]) {
		if (value == null)
			return("NULL");
		
		StringBuilder sb = new StringBuilder();
		for (int i=0; i<value.length; i++)
			sb.append((char)value[i]);
		return(sb.toString());
	}
	
	public static String printArrayAsHEX(byte value[]) {
		return(printArrayAsHEX(value,0,value.length));
	}
	
	public static String printArrayAsHEX(byte value[], int offset, int expectedLen) {
		if (value.length < (offset+expectedLen))
			return("ERRLEN");

		StringBuilder sb = new StringBuilder();
		for (int i=0; i<expectedLen; i++)
			sb.append(printByteHex(value[i+offset]));

		return(sb.toString());
	}

	public static String printArrayAsUnsignedInt(byte value[], int offset, int expectedLen) {
		if (value.length < (offset+expectedLen))
			return("ERRLEN");

		byte t_array[] = new byte[8];
		
		for (int i=0; i<expectedLen; i++)
			t_array[i] = value[i+offset];
		for (int i=expectedLen; i<t_array.length; i++)
			t_array[i] = 0x00;
		
		Logger.getGlobal().log(Level.FINEST, "*** printing UNSIGNED INT "+printArrayAsHEX(t_array, 0, 8));

		ByteBuffer bb = ByteBuffer.wrap(t_array);
		bb.order(ByteOrder.LITTLE_ENDIAN);
		long t_val = bb.getLong();
		return(String.valueOf(t_val));
	}

	public static String printArrayAsInt(byte value[], int offset, int expectedLen) {
		if (value.length < (offset+expectedLen))
			return("ERRLEN");

		byte t_array[] = new byte[8];
		for (int i=0; i<expectedLen; i++)
			t_array[i] = value[i+offset];
		if ((offset+expectedLen) < value.length) {
			if ((t_array[expectedLen-1] & 0x80) == 0) {
				// negative int
				for (int i=expectedLen; i<t_array.length; i++)
					t_array[i] = (byte) 0x00;
			} else {
				// positive int
				for (int i=expectedLen; i<t_array.length; i++)
					t_array[i] = (byte) 0xFF;
			}
		}

		Logger.getGlobal().log(Level.FINEST, "*** printing INT("+value.length+","+offset+","+expectedLen+") "+printArrayAsHEX(value)+"=> "+printArrayAsHEX(t_array,0, 4));
		
		ByteBuffer bb = ByteBuffer.wrap(t_array);
		bb.order(ByteOrder.LITTLE_ENDIAN);
		int t_val = bb.getInt();
		Logger.getGlobal().log(Level.FINEST, "*** int = "+t_val);
		return(String.valueOf(t_val));
	}

	public static String printArrayAsFloat(byte value[]) {
		if (value.length != 4)
			return("ERRLEN");

		ByteBuffer bb = ByteBuffer.wrap(value);
		bb.order(ByteOrder.LITTLE_ENDIAN);
		float t_val = bb.getFloat();
		return(String.valueOf(t_val));
	}
	
	public static String printHByteHex(byte b) {
		StringBuilder sb = new StringBuilder();
		sb.append(Integer.toHexString((b&0xF0)>>4));
		return(sb.toString());
	}

	public static String printLByteHex(byte b) {
		StringBuilder sb = new StringBuilder();
		sb.append(Integer.toHexString(b&0x0F));
		return(sb.toString());
	}

	public static String printByteHex(byte b) {
		StringBuilder sb = new StringBuilder();
		if ((b & 0xF0) == 0)
			sb.append("0");
		sb.append(Integer.toHexString(b&0xFF));
		return(sb.toString());
	}

	public static String printShortHex(short s) {
		// TODO : big endian vs little endian ?
		StringBuilder sb = new StringBuilder();
		if ((s & 0xF000) == 0)
			sb.append("0");
		if ((s & 0xFF00) == 0)
			sb.append("0");
		if ((s & 0xFFF0) == 0)
			sb.append("0");
		sb.append(Integer.toHexString((int)(s&0xFFFF)));
		return(sb.toString());
	}
	
	public static void main(String[] args) {
		System.out.println("01 => "+Print.printByteHex((byte)0x01));
		System.out.println("11 => "+Print.printByteHex((byte)0x11));
		System.out.println("F1 => "+Print.printByteHex((byte)0xF1));
		System.out.println("0102 => "+Print.printShortHex((short)0x0102));
		System.out.println("1110 => "+Print.printShortHex((short)0x1110));
		System.out.println("F101 => "+Print.printShortHex((short)0xF101));
	}	
}
