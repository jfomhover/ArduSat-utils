package datalib.syntax;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import datalib.headers.ChunkHeader;
import datalib.headers.LogHeader;
import datalib.headers.SerieHeader;
import datalib.headers.UserHeader;
import datalib.packets.ChunkPacket;
import datalib.packets.LogPacket;
import datalib.packets.Packet;
import datalib.packets.SeriePacket;
import datalib.packets.UserPacket;

public class SyntaxManager {
	public static final Map<Short,DataType> dataTypes = new HashMap<Short, DataType>();
    static {
    	dataTypes.put((short)0x0001, 	new DataType("MS", 			 (short)0x0001, "MS", 			Arrays.asList("MS"), 4));
    	dataTypes.put((short)0x0002, 	new DataType("LUM1", 		 (short)0x0002, "LUM1", 		Arrays.asList("LUM1_IR","LUM1_IR"), 4));
    	dataTypes.put((short)0x0004,	new DataType("LUM2", 		 (short)0x0004, "LUM2",			Arrays.asList("LUM2_IR","LUM2_IR"), 4));
    	dataTypes.put((short)0x0008,	new DataType("MAG", 		 (short)0x0008, "MAG", 			Arrays.asList("MAG_X","MAG_Y","MAG_Z"), 6));
    	dataTypes.put((short)0x0010,	new DataType("TMP1", 		 (short)0x0010, "TMP1", 		Arrays.asList("TMP1"), 2));
    	dataTypes.put((short)0x0020,	new DataType("TMP2", 		 (short)0x0020, "TMP2", 		Arrays.asList("TMP2"), 2));
    	dataTypes.put((short)0x0040,	new DataType("TMP3", 		 (short)0x0040, "TMP3", 		Arrays.asList("TMP3"), 2));
    	dataTypes.put((short)0x0080,	new DataType("TMP4", 		 (short)0x0080, "TMP4", 		Arrays.asList("TMP4"), 2));
    	dataTypes.put((short)0x0100, 	new DataType("INFRATHERM", 	 (short)0x0100, "INFRATHERM",	Arrays.asList("INFRATHERM"), 2));
    	dataTypes.put((short)0x0200,	new DataType("ACCEL", 		 (short)0x0200, "ACCEL", 		Arrays.asList("ACCEL_X","ACCEL_Y","ACCEL_Z"), 6));
    	dataTypes.put((short)0x0400,	new DataType("GYRO", 		 (short)0x0400, "GYRO", 		Arrays.asList("GYRO_X","GYRO_Y","GYRO_Z"), 6));
    	dataTypes.put((short)0x0800,	new DataType("GEIGER1", 	 (short)0x0800, "GEIGER1", 		Arrays.asList("GEIGER1"), 0));
    	dataTypes.put((short)0x1000,	new DataType("GEIGER2", 	 (short)0x1000, "GEIGER2", 		Arrays.asList("GEIGER2"), 0));
    	dataTypes.put((short)0x2000, 	new DataType("USERDEFINED1", (short)0x2000, "USERDEFINED1", Arrays.asList("UD1"), 5));
    	dataTypes.put((short)0x4000, 	new DataType("USERDEFINED2", (short)0x4000, "USERDEFINED2", Arrays.asList("UD2"), 5));
    	dataTypes.put((short)0x8000,	new DataType("CRC16", 		 (short)0x8000, "CRC16", 		Arrays.asList("CRC16"), 2));
    };
    
    public static final Map<Byte, Packet> packets = new HashMap<Byte, Packet>();
    static {
    	packets.put(LogHeader.HEADERBYTE, new LogPacket());
    	packets.put(ChunkHeader.HEADERBYTE, new ChunkPacket());
    	packets.put(SerieHeader.HEADERBYTE, new SeriePacket());
    	packets.put(UserHeader.HEADERBYTE, new UserPacket());
    };
    
    public static Packet getPacket(byte code) {
    	if (packets.containsKey(code))
    		return(packets.get(code));
    	else
    		return(null);
    };
    
    public static List<DataType> searchTypes(short code) {
    	List<DataType> t_ret = new ArrayList<DataType>();
    	for (int i=0; i<16; i++) {
    		short lcode = (short) (1 << i);
    		if ((lcode & code) != 0)
    			t_ret.add(dataTypes.get(lcode));
    	}
    	return(t_ret);
    }
    
    public static List<String> searchTypesSubLabels(short code) {
    	List<String> t_ret = new ArrayList<String>();
    	for (int i=0; i<16; i++) {
    		short lcode = (short) (1 << i);
    		if ((lcode & code) != 0)
    			t_ret.addAll(dataTypes.get(lcode).getSublabels());
    	}
    	return(t_ret);
    }

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		List<DataType> test = SyntaxManager.searchTypes((short)0xFFFF);
		Iterator<DataType> iter = test.iterator();
		while (iter.hasNext())
			System.out.println(iter.next().toString());
	}
}
