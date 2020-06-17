package gui;


import java.awt.Toolkit;
import java.io.IOException;

public class Main {
	
	public static final int resizeTO = 12;
	public static final int pca = 20;
	
	public static void main(String[] args) {
		try {
			Percepcao percepcao = new Percepcao(resizeTO, pca);
			int x = Toolkit.getDefaultToolkit().getScreenSize().width;
			int y = Toolkit.getDefaultToolkit().getScreenSize().height;
			x = (x - 300) / 2;
			y = (y - 200) / 2;
			percepcao.setLocation(x, y);
			percepcao.setVisible(true);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		/*try {
			PhotoToVector.run(resizeTO);
			SubtraiMedia.run();
			for (int i = 10; i <= 25; i = i + 1) {
				PCA.run(i);
				System.out.println("-------------------------------------------------------------------------");
				System.out.println("Resize = " + resizeTO + "\nPCA = " + i);
				TesteCompleto.run(1, 1, false);
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}*/
	}
}
