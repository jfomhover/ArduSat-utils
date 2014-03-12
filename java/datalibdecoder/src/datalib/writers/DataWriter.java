package datalib.writers;
import java.io.IOException;
import java.util.List;

public interface DataWriter {
    void openDataType(short code) throws IOException;
    void onDataTypeChange(short code) throws IOException;
    void onDataTypeEntry(short code, List<String> values) throws IOException;
    void closeDataType(short code) throws IOException;
    
    void openUserData() throws IOException;
    void closeUserData() throws IOException;

    void onDataString(String val) throws IOException;
    
    void openSerieEntry(byte keyType, int keyDim, byte valType, int valDim) throws IOException;
    void onNewSerie(byte keyType, int keyDim, byte valType, int valDim) throws IOException;
    void onSerieData(String val) throws IOException;
    void closeSerieEntry() throws IOException;
    
    void onLogString(String val) throws IOException;
}
