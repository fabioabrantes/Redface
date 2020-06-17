package preProcess.leitura;

import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;

import util.Arquivo;
import util.Util_Imagem;

public class PhotoToVector {

	public static final int numClasses = 64;
	public static final int fotosTreino = 5;
	public static int width;
	public static int height;
	public static int resize;
	private static double[] media;
	
	private static void lerBase(Arquivo treino, Arquivo teste, String diretorio) throws IOException{
		for (int i = 0; i < numClasses; i++) {
			String labelPasta = "s" + (i + 1);
			String pasta = diretorio + labelPasta;
			for (int j = 0; j < 5; j++) {
				String photo = pasta + "/_" + (j + 1) + ".jpg";
				imprimePhoto(photo, treino, labelPasta, true);
			}
			for (int j = 0; j < 15; j++) {
				String photo = pasta + "/(" + (j + 1) + ").jpg";
				imprimePhoto(photo, teste, labelPasta, false);
			}
			System.out.println(labelPasta + " terminada!");
		}
		treino.close();
		teste.close();
		System.out.println("Leitura terminada!");
	}
	
	private static void imprimePhoto(String photo, Arquivo arq, String labelPhoto, boolean treino) throws IOException{
		File thePhoto = new File(photo);
		File resizedPhoto = new File(photo.replaceFirst(".jpg", "_resized.jpg"));
		Util_Imagem.resize(thePhoto, resizedPhoto, resize, 1f);
		BufferedImage image = ImageIO.read(resizedPhoto);
		int[] pixelsPhoto = Util_Imagem.pegarPixels(image);
		for (int k = 0; k < pixelsPhoto.length; k++) {
			double temp = (double)pixelsPhoto[k];
			if(treino){
				media[k] = media[k] + temp;
			}
			arq.print(temp + " ");
		}
		arq.println(labelPhoto);
	}

	public static void run(int resizeTO) throws IOException {
		resize = resizeTO;
		width = resize;
		height = (resize * 90) / 120;
		media = new double[width * height];
		
		Arquivo arqTreino = new Arquivo("fool.txt", "treino.txt");
		Arquivo arqTeste = new Arquivo("fool.txt", "teste.txt");
		
		lerBase(arqTreino, arqTeste, "outputFaces120x90_separada/");
		
		System.out.println("Calculando média...");
		Arquivo arqMedia = new Arquivo("fool.txt", "media.txt");
		int numExemplosTreino = numClasses * fotosTreino;
		for (int i = 0; i < media.length; i++) {
			arqMedia.println(media[i]/numExemplosTreino);
		}
		arqMedia.close();
		System.out.println("Finalizado!");
	}
}
