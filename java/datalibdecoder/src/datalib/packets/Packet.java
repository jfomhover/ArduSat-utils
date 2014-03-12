package datalib.packets;

import java.io.IOException;
import java.io.InputStream;

import datalib.headers.PacketHeaderInterface;
import datalib.writers.DataWriter;

public abstract class Packet {
	protected PacketHeaderInterface hdr = null;
	
	protected void setPacketHeader(PacketHeaderInterface hdr) { this.hdr = hdr; };
	
	public PacketHeaderInterface getHeader() { return(hdr); };
	public byte getHeaderCode() { return(hdr.getHeaderCode()); };
	
	protected boolean decodeHeader(byte code, InputStream istr) throws IOException {
		if (hdr == null)
			return(false);		
		if (hdr.getHeaderLength() == 0)
			return(false);
		if (code != hdr.getHeaderCode())
			return(false);
		
		byte hdr_array[] = new byte[hdr.getHeaderLength()];
		hdr_array[0] = code;
		
		int t_ret = istr.read(hdr_array, 1, hdr.getHeaderLength()-1);
		if (t_ret < (hdr.getHeaderLength()-1))
			return(false);

		return(hdr.decode(hdr_array));
	}
	
	public abstract boolean decode(byte code, InputStream istr, DataWriter dw) throws IOException;
}
