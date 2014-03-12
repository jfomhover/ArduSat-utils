package datalib.syntax;

import java.util.Iterator;
import java.util.List;

public class DataType {
	String id;
	short hexcode;
	String label;
	List<String> sublabels;
	int dataSize;
	
	public DataType(String id, short hexcode, String label, List<String> sublabels, int dataSize) {
		this.id = id;
		this.hexcode = hexcode;
		this.label = label;
		this.sublabels = sublabels;
		this.dataSize = dataSize;
	}
	
	public String getId() { return(id); };
	public short getHexCode() { return(hexcode); };
	public String getLabel() { return(label); };
	public List<String> getSublabels() { return(sublabels); };
	public int getDataSize() { return(dataSize); };

	public static short hexcode(List<DataType> input) {
		Iterator<DataType> iter = input.iterator();
		short t_ret = 0;
		while (iter.hasNext()) {
			t_ret |= iter.next().hexcode;
		}
		return(t_ret);
	}
	
	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append("DT{");
		sb.append(id);
		sb.append(",");
		sb.append(hexcode);
		sb.append(",");
		sb.append(label);
		sb.append(",");
		sb.append("{");
		Iterator<String> iter = sublabels.iterator();
		while(iter.hasNext()) {
			sb.append(" ");
			sb.append(iter.next());
		}
		sb.append(" },");
		sb.append(dataSize);
		return(sb.toString());
	}
}
