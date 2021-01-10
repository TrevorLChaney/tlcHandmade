import javax.swing.JFrame;

public class HistogramViewer {
	public static void main(String[] args) {
		JFrame appFrame = new JFrame("New Histogram");
		HistogramComponent histogramComponent = new HistogramComponent();

		appFrame.setSize(400, 300);
		appFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		appFrame.add(histogramComponent);
		appFrame.setVisible(true);
	}
}

