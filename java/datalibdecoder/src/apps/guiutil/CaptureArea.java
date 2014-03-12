package apps.guiutil;

import java.awt.EventQueue;
import javax.swing.JTextArea;

public class CaptureArea {

	private JTextArea output;

	public CaptureArea(JTextArea destination) {
        	this.output = destination;
//            setLayout(new BorderLayout());
//            output = new JTextArea();
//            add(new JScrollPane(output));
	}

	public void appendText(final String text) {
		if (EventQueue.isDispatchThread()) {
			output.append(text);
			output.setCaretPosition(output.getText().length());
		} else {
			EventQueue.invokeLater(new Runnable() {
				@Override
				public void run() {
					appendText(text);
				}
			});
		}
	}        
}
