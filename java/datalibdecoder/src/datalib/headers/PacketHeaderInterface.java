package datalib.headers;

public interface PacketHeaderInterface {
	int getHeaderLength();
	byte getHeaderCode();
	int getPacketLength();
	int getBodyLength();
	boolean decode(byte array[]);
}
