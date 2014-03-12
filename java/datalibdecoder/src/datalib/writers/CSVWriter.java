package datalib.writers;

import java.io.IOException;
import java.io.OutputStream;
import java.util.List;

import datalib.syntax.SyntaxManager;

enum CSVState { CLOSED, OPEN, DATATYPE, LOG, SERIE, SERIEENTRY };

public class CSVWriter implements DataWriter {
	public static final char DEFAULT_SEPARATION = ';';
	public static final char DEFAULT_BR = '\n';
	
	short currentDTCode = 0;
	char _separation = DEFAULT_SEPARATION;
	OutputStream ostr = System.out;
	CSVState state = CSVState.CLOSED;
	
	public void setSeparationChar(char c) {
		this._separation = c;
	}
	
	private void outputStringSeparated(String str) throws IOException {
		ostr.write(str.getBytes());
		ostr.write(_separation);
	}
	
	public void setOutputStream(OutputStream ostr) {
		this.ostr = ostr;
	}
	

	@Override
	public void openDataType(short code) throws IOException {
		if (currentDTCode != code)
			onDataTypeChange(code);
	}
	
	@Override
	public void onDataTypeEntry(short code, List<String> values) throws IOException {
		for (int i=0; i<values.size(); i++)
			onDataString(values.get(i));
	}

	@Override
	public void closeDataType(short code) throws IOException {
		ostr.write(DEFAULT_BR);
	}

	@Override
	public void onDataTypeChange(short code) throws IOException {
		currentDTCode = code;
		List<String> t_list = SyntaxManager.searchTypesSubLabels(code);
		for (int i=0; i<t_list.size(); i++) {
			ostr.write(t_list.get(i).getBytes());
			ostr.write(_separation);
		}
		ostr.write(DEFAULT_BR);
	}

	@Override
	public void onDataString(String val) throws IOException {
		outputStringSeparated(val);
	}

	int _currentSerie[] = null;

	public void onNewSerie(byte keyType, int keyDim, byte valType, int valDim) throws IOException {
		StringBuilder sb = new StringBuilder();
		for (int i=0; i<keyDim; i++) {
			sb.append("KEY_"+i);
			sb.append(_separation);
		}
		for (int i=0; i<valDim; i++) {
			sb.append("VAL_"+i);
			sb.append(_separation);
		}
		ostr.write(sb.toString().getBytes());
		ostr.write(DEFAULT_BR);
	}
	
	@Override
	public void openSerieEntry(byte keyType, int keyDim, byte valType, int valDim) throws IOException {
		if (_currentSerie == null) {
			_currentSerie = new int[4];
			_currentSerie[0] = keyType;
			_currentSerie[1] = keyDim;
			_currentSerie[2] = valType;
			_currentSerie[3] = valDim;
			onNewSerie(keyType,keyDim,valType,valDim);
		} else {
			if ( (_currentSerie[0] != keyType) ||
				 (_currentSerie[1] != keyDim) ||
				 (_currentSerie[2] != valType) ||
				 (_currentSerie[3] != valDim) ) 	{
				_currentSerie[0] = keyType;
				_currentSerie[1] = keyDim;
				_currentSerie[2] = valType;
				_currentSerie[3] = valDim;
				onNewSerie(keyType,keyDim,valType,valDim);
			}
		}
	}

	@Override
	public void onSerieData(String val) throws IOException {
		outputStringSeparated(val);
	}

	@Override
	public void closeSerieEntry() throws IOException {
		ostr.write(DEFAULT_BR);
	}

	@Override
	public void onLogString(String val) throws IOException {
//		System.out.println("CSVWriter:onLogString()"+val);
		String logcontent = new String("###LOG"+_separation+val);
		ostr.write(logcontent.getBytes());
		ostr.write(DEFAULT_BR);
	}

	@Override
	public void openUserData() throws IOException {
		String content = new String("###USER"+_separation);
		ostr.write(content.getBytes());
	}

	@Override
	public void closeUserData() throws IOException {
		ostr.write(DEFAULT_BR);
	}

}
