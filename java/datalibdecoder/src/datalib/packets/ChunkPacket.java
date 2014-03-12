package datalib.packets;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;

import datalib.headers.ChunkHeader;
import datalib.syntax.DataType;
import datalib.syntax.GenericUnit;
import datalib.syntax.SyntaxManager;
import datalib.util.Print;
import datalib.writers.CSVWriter;
import datalib.writers.DataWriter;

public class ChunkPacket extends Packet {

	public ChunkPacket() {
		super();
		setPacketHeader(new ChunkHeader());
	}

	@Override
	public boolean decode(byte code, InputStream istr, DataWriter dw) throws IOException {
		if (!decodeHeader(code, istr))
			return(false);
		
		short t_datatypes = (short)((ChunkHeader)hdr).getDataTypes();
		
		if (dw != null)
			dw.openDataType(t_datatypes);

		List<DataType> t_list = SyntaxManager.searchTypes(t_datatypes);
//		System.out.println("Found "+t_list.size()+"datatypes in header size "+hdr.getHeaderLength());
		
		for (int i=0; i<t_list.size(); i++) {
			int t_size = t_list.get(i).getDataSize();
			byte t_array[] = new byte[t_size];
			if (istr.read(t_array) < t_size)
				return(false);
			
			short t_code = t_list.get(i).getHexCode();
			String t_id = t_list.get(i).getId();
			List<String> t_values = new ArrayList<String>();
			
			switch(t_id) {
			case "MS":
				t_values.add(Print.printArrayAsInt(t_array, 0, 4));
				break;
			case "LUM1":
			case "LUM2":
				t_values.add(Print.printArrayAsUnsignedInt(t_array, 0, 2));
				t_values.add(Print.printArrayAsUnsignedInt(t_array, 2, 2));
				break;
			case "MAG":
			case "GYRO":
			case "ACCEL":
				t_values.add(Print.printArrayAsInt(t_array, 0, 2));
				t_values.add(Print.printArrayAsInt(t_array, 2, 2));
				t_values.add(Print.printArrayAsInt(t_array, 4, 2));
				break;
			case "TMP1":
			case "TMP2":
			case "TMP3":
			case "TMP4":
			case "INFRATHERM":
				t_values.add(Print.printArrayAsInt(t_array, 0, 2));
				break;
			case "USERDEFINED1":
			case "USERDEFINED2":
				GenericUnit t_unit = new GenericUnit();
				t_unit.setCode(t_array[0]);
				t_unit.forceDimension(1);
				t_unit.decode(t_array[0], t_array, 1);
				for (int vi=0; vi<t_unit.getDimension(); vi++)
					t_values.add(t_unit.getValue(vi));
				break;
			default:
				break;
			}
			
			if (dw != null)
				dw.onDataTypeEntry(t_code, t_values);
		}
		
		if (dw != null)
			dw.closeDataType(code);
		
		return true;
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

		ChunkPacket pckt = new ChunkPacket();
		
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
