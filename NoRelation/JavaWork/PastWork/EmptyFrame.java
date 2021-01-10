import javax.swing.JFrame;

public class EmptyFrame {
	public static void main(String[] args) {
		JFrame appFrame = new JFrame("New Empty Window");
		appFrame.setSize(400, 300);
		appFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		appFrame.setVisible(true);
	}
}
