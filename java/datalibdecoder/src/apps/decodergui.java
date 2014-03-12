package apps;
import java.awt.EventQueue;

import javax.swing.JFrame;
import javax.swing.JPanel;

import java.awt.BorderLayout;

import javax.swing.BoxLayout;
import javax.swing.JFileChooser;
import javax.swing.JTabbedPane;
import javax.swing.JTextArea;
import javax.swing.JMenuBar;
import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;

import java.awt.FlowLayout;
import java.awt.Component;

import javax.swing.JButton;
import javax.swing.JTextField;
import javax.swing.JTextPane;
import javax.swing.filechooser.FileNameExtensionFilter;

import datalib.writers.CSVWriter;
import decoders.FileDecoder;

import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import java.io.ByteArrayOutputStream;
import java.io.PrintStream;
import java.util.logging.Logger;

import javax.swing.JScrollPane;

import java.awt.Font;

import javax.swing.JRadioButton;

import apps.guiutil.CaptureArea;
import apps.guiutil.StreamCapturer;


public class decodergui {

	private JFrame frame;
	private JTextField filenameField;
	private JTextArea textOutputArea;
	
	/**
	 * Launch the application.
	 */
	public static void main(String[] args) {
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					decodergui window = new decodergui();
					window.frame.setVisible(true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}

	/**
	 * Create the application.
	 */
	public decodergui() {
		initialize();
	}

	/**
	 * Initialize the contents of the frame.
	 */
	private void initialize() {
		try {
            UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
        } catch (ClassNotFoundException ex) {
        } catch (InstantiationException ex) {
        } catch (IllegalAccessException ex) {
        } catch (UnsupportedLookAndFeelException ex) {
        }
		
		frame = new JFrame();
		frame.setBounds(100, 100, 640, 480);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.getContentPane().setLayout(new BorderLayout(0, 0));
		
		JPanel panel_2 = new JPanel();
		frame.getContentPane().add(panel_2, BorderLayout.NORTH);
		panel_2.setLayout(new BoxLayout(panel_2, BoxLayout.X_AXIS));
		
		JButton processButton = new JButton("Process");
		processButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				String filepath = filenameField.getText();
				if (filepath.length() < 1)
					return;
				
				ByteArrayOutputStream bosOut = new ByteArrayOutputStream();
				//ByteArrayOutputStream bosErr = new ByteArrayOutputStream();
				
				FileDecoder fd = new FileDecoder();
				
				fd.setFilename(filepath);
				fd.open();
				
				CSVWriter csvWriter = new CSVWriter();
				csvWriter.setSeparationChar(';'); //\t');
				csvWriter.setOutputStream(bosOut);
				
				while(fd.available())
					fd.decodePacket(csvWriter);
				
				textOutputArea.setText(bosOut.toString());
			}
		});
		panel_2.add(processButton);
		
		JButton loadButton = new JButton("Load file");
		loadButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				JFileChooser chooser = new JFileChooser();
			    FileNameExtensionFilter filter = new FileNameExtensionFilter(
			        "binary files", "BIN");
			    chooser.setFileFilter(filter);
			    int returnVal = chooser.showOpenDialog(frame);
			    if(returnVal == JFileChooser.APPROVE_OPTION) {
			       System.out.println("You chose to open this file: " +
			            chooser.getSelectedFile().getName());
			       filenameField.setText(chooser.getSelectedFile().getAbsolutePath());
			    }
			}
		});
		panel_2.add(loadButton);
		
		filenameField = new JTextField();
		panel_2.add(filenameField);
		filenameField.setColumns(10);
		
		JTabbedPane tabbedPane = new JTabbedPane(JTabbedPane.TOP);
		tabbedPane.setAlignmentY(Component.BOTTOM_ALIGNMENT);
		frame.getContentPane().add(tabbedPane, BorderLayout.CENTER);
		
		JPanel panel = new JPanel();
		tabbedPane.addTab("Output", null, panel, null);
		panel.setLayout(new BoxLayout(panel, BoxLayout.X_AXIS));
		
		JScrollPane scrollPane = new JScrollPane();
		panel.add(scrollPane);
		
		textOutputArea = new JTextArea();
		textOutputArea.setFont(new Font("Courier New", Font.PLAIN, 13));
		scrollPane.setViewportView(textOutputArea);
		
		JPanel panel_1 = new JPanel();
		tabbedPane.addTab("Log", null, panel_1, null);
		panel_1.setLayout(new BoxLayout(panel_1, BoxLayout.X_AXIS));
		
		JScrollPane scrollPane_1 = new JScrollPane();
		panel_1.add(scrollPane_1);
		
		JTextArea logArea = new JTextArea();
		scrollPane_1.setViewportView(logArea);
		logArea.setText("");

		PrintStream ps = System.out;
        CaptureArea logCaptureArea = new CaptureArea(logArea);
        System.setErr(new PrintStream(new StreamCapturer("STDERR", logCaptureArea, ps)));
        System.setOut(new PrintStream(new StreamCapturer("STDOUT", logCaptureArea, ps)));
	}

}
