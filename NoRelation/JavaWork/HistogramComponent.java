import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import javax.swing.JComponent;

public class HistogramComponent extends JComponent {
	@Override
	public void paintComponent(Graphics g) {
		Graphics2D graphicsObj = (Graphics2D)g;

		Rectangle binRectangle1 = new Rectangle(10, 10, 200, 50);
		Color binColor1 = new Color(128, 128, 0);
		graphicsObj.setColor(binColor1);
		graphicsObj.fill(binRectangle1);

		Rectangle binRectangle2 = new Rectangle(10, 75, 150, 50);
		Color binColor2 = new Color(0, 200, 200);
		graphicsObj.setColor(binColor2);
		graphicsObj.fill(binRectangle2);

		Rectangle binRectangle3 = new Rectangle(10, 140, 350, 50);
		Color binColor3 = new Color(100, 100, 100);
		graphicsObj.setColor(binColor3);
		graphicsObj.draw(binRectangle3);
	}
}
