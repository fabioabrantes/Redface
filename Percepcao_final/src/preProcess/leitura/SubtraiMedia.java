package preProcess.leitura;

import java.io.IOException;

import util.Arquivo;

public class SubtraiMedia {
	
	public static final int numAtributos = PhotoToVector.width * PhotoToVector.height;
	public static final int numExemplosTreino = PhotoToVector.numClasses * PhotoToVector.fotosTreino;

	public static void run() throws IOException {
		double[] medias = new double[numAtributos];
		Arquivo arqTreinoMedia = new Arquivo("treino.txt", "treinoMenosMedia.txt");
		Arquivo arqMedia = new Arquivo("media.txt", "fool.txt");
		for (int i = 0; i < medias.length; i++) {
			medias[i] = arqMedia.readDouble();
		}
		System.out.println("Leiura das médias terminada!");
		
		for (int i = 0; i < numExemplosTreino; i++) {
			for (int j = 0; j < numAtributos; j++) {
				double resultado = arqTreinoMedia.readDouble() - medias[j];
				arqTreinoMedia.print(resultado + " ");
			}
			arqTreinoMedia.println(arqTreinoMedia.readString());
		}
		arqTreinoMedia.close();
		arqMedia.close();
		System.out.println("Subtração dos atributos pelas médias concluída!");
	}

}
