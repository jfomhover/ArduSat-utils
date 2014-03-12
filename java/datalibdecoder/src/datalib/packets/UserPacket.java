package datalib.packets;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;

import datalib.headers.UserHeader;
import datalib.syntax.DataType;
import datalib.syntax.GenericUnit;
import datalib.syntax.SyntaxManager;
import datalib.util.Print;
import datalib.writers.CSVWriter;
import datalib.writers.DataWriter;

public class UserPacket extends Packet {

	public UserPacket() {
		super();
		setPacketHeader(new UserHeader());
	}

	@Override
	public boolean decode(byte code, InputStream istr, DataWriter dw) throws IOException {
		if (!decodeHeader(code, istr))
			return(false);
		
		dw.openUserData();
		
		GenericUnit t_unit = new GenericUnit();
		
		byte t_array[] = new byte[hdr.getBodyLength()]; // TODO error checking
		int t_len = istr.read(t_array);
		if (t_len < hdr.getBodyLength())
			return(false);
		
		int t_offset = 0;
		while (t_offset < t_len) {
			t_unit.setCode(t_array[t_offset]);
			if (t_unit.getDimension() == 0)
				t_unit.forceDimension(1);
			t_unit.decode(t_array[t_offset], t_array, t_offset+1);
			
			for (int i=0; i<t_unit.getDimension(); i++)
				dw.onDataString(t_unit.getValue(i));
			
			t_offset += 1 + t_unit.getLength();
		}
		
		dw.closeUserData();
		return true;
	}
}
