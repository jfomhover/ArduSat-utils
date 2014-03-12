package datalib.syntax;

import datalib.util.Print;

public class GenericUnit {
	public static final byte UNIT_HEX1 	= 0x00;
	public static final byte UNIT_HEX2 	= 0x01;
	public static final byte UNIT_HEX3 	= 0x02;
	public static final byte UNIT_HEX4 	= 0x03;
	public static final byte UNIT_INT1 	= 0x04;
	public static final byte UNIT_INT2 	= 0x05;
	public static final byte UNIT_INT3 	= 0x06;
	public static final byte UNIT_INT4 	= 0x07;
	public static final byte UNIT_UINT1 = 0x08;
	public static final byte UNIT_UINT2 = 0x09;
	public static final byte UNIT_UINT3 = 0x0A;
	public static final byte UNIT_UINT4 = 0x0B;
	public static final byte UNIT_STR 	= 0x0D;
	public static final byte UNIT_FLOAT = 0x0F;

	byte code;
	byte type;
	int length;
	int dimension;
//	String label;
	byte value[];
	
	public GenericUnit() {
		setCode(UNIT_HEX4);
	}
	
	public byte getCode() { return(code); };
	public int getUnitSize() { return(length); };
	public int getLength() { return(length*dimension); };
	public int getType() { return(type); };
	public int getDimension() { return(dimension); };
//	public String getLabel() { return(label); };
	public byte[] getValue() { return(value); };

	public void setCode(byte code) {
		this.code = code;

		this.type = (byte) (code & 0x0F);

		if (this.type == UNIT_STR)
			this.length = 4;
		else
			this.length = (code & 0x03)+1;
//		this.dimension = (code & 0xF0) >> 4;

		forceDimension((code & 0xF0) >> 4);
	}
	
	public void forceDimension(int dimension) {
		this.dimension = dimension;
		if (this.dimension > 0)
			if (this.value == null) {
				this.value = new byte[length*dimension];
			} else if (this.value.length < (length*dimension)) {
				this.value = new byte[length*dimension];
			}
	}
	
	public boolean decode(byte code, byte value[], int offset) {
		if (this.code != code)
			return(false);
//			setCode(code);
		
		if (value.length < (length*dimension))	// TODO : offset here !!!
			return(false);
	
		for (int i=0; i<(length*dimension); i++)
			this.value[i] = value[i+offset];
		
		return(true);
	}
	
	public String getValue(int i) {
		if ((i >= 0) && (i < dimension))
			return(singleValueToString(i));
		return "ERR";
	}
	
	public String toString() {
		StringBuilder sb = new StringBuilder();
		for (int i=0; i<dimension; i++) {
			if (i > 0)
				sb.append(Print.DEFAULT_SEPARATOR);
			sb.append(singleValueToString(i));
		}
		return(sb.toString());
	}
	
	private String singleValueToString(int dim) {
		if (dimension == 0)
			return("ERR");
		if (length == 0)
			return("ERR");
		
		byte t_array[] = new byte[length];
		for (int i=0; i<length; i++)
			t_array[i] = value[dim*length+i];
		
		switch(type) {
		case UNIT_STR:
			return(Print.printArrayAsSTR(t_array));
		case UNIT_FLOAT:
			return(Print.printArrayAsFloat(t_array));
		case UNIT_INT1:
		case UNIT_INT2:
		case UNIT_INT3:
		case UNIT_INT4:
			return(Print.printArrayAsInt(t_array, 0, (code&0x03)+1));
		case UNIT_UINT1:
		case UNIT_UINT2:
		case UNIT_UINT3:
		case UNIT_UINT4:
			return(Print.printArrayAsUnsignedInt(t_array, 0, (code&0x03)+1));
		case UNIT_HEX1:
		case UNIT_HEX2:
		case UNIT_HEX3:
		case UNIT_HEX4:
		default:
			return(Print.printArrayAsHEX(t_array, 0, (code&0x03)+1));
		}
	}
}
