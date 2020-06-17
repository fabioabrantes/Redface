package classificacao.knn;

import java.util.ArrayList;
import java.util.Iterator;

import preProcess.PCA.PCA;
import preProcess.leitura.PhotoToVector;
import preProcess.leitura.SubtraiMedia;
import util.Arquivo;
import Jama.Matrix;
import classificacao.basico.Base;
import classificacao.basico.Padrao;

public class TesteCompleto {

	public static ArrayList<Double> medias;
	public static Matrix autoVetores;
	public static Base baseTreino;
	
	public static String run(int k_inicial, int k_final, boolean peso) {
		carregaAutoVetores();
		
		carregaBaseTreino();
		
		carregaMedias();
		
		Arquivo arqBaseTeste = new Arquivo("teste.txt", "fool.txt");
		int numExemplosTeste = PhotoToVector.numClasses * (20 - PhotoToVector.fotosTreino);
		ArrayList<Padrao> baseTeste = new ArrayList<Padrao>(numExemplosTeste);
		System.out.println("Testando...");
		for (int i = 0; i < numExemplosTeste; i++) {
			Matrix padrao_teste = new Matrix(1, SubtraiMedia.numAtributos);
			for (int j = 0; j < SubtraiMedia.numAtributos; j++) {
				double resultado = arqBaseTeste.readDouble() - medias.get(j);
				padrao_teste.set(0, j, resultado);
			}
			padrao_teste = padrao_teste.times(autoVetores);
			double[] array = padrao_teste.getArray()[0];
			Padrao novoPadraoTeste = new Padrao(PCA.dimensaoPCA);
			for (int j = 0; j < PCA.dimensaoPCA; j++) {
				novoPadraoTeste.atributos.add(j, array[j]);
			}
			novoPadraoTeste.classe = arqBaseTeste.readString();
			baseTeste.add(novoPadraoTeste);
		}
		String retorno = "";
		for (int i = k_inicial; i <= k_final; i++) {
			int corretas = 0;
			for (Iterator<Padrao> iterator = baseTeste.iterator(); iterator.hasNext();) {
				Padrao novo = iterator.next();
				String classificacao = KNN.k_nn(i, novo, baseTreino, peso);
			
				if(classificacao.equals(novo.classe)){
					corretas++;
				}
			}
			System.out.println("Acertos (" + i + "): " + (corretas / (double)numExemplosTeste * 100) + "%");
			
			retorno = (corretas / (double)numExemplosTeste * 100) + "%";
		}
		return retorno;
	}

	public static void carregaMedias() {
		Arquivo arqMedias = new Arquivo("media.txt", "fool.txt");
		medias = new ArrayList<Double>(SubtraiMedia.numAtributos);
		for (int i = 0; i < SubtraiMedia.numAtributos; i++) {
			medias.add(arqMedias.readDouble());
		}
		arqMedias.close();
	}

	public static void carregaAutoVetores() {
		Arquivo arqAutoVetor = new Arquivo("autoVetores.txt", "fool.txt");
		autoVetores = new Matrix(SubtraiMedia.numAtributos, PCA.dimensaoPCA);
		for (int i = 0; i < SubtraiMedia.numAtributos; i++) {
			for (int j = 0; j < PCA.dimensaoPCA; j++) {
				autoVetores.set(i, j, arqAutoVetor.readDouble());
			}
		}
		arqAutoVetor.close();
		System.out.println("Auto-vetores carregados!");
	}

	public static void carregaBaseTreino() {
		Arquivo arqBaseTreino = new Arquivo("treinoFinal.txt", "fool.txt");
		baseTreino = new Base(SubtraiMedia.numExemplosTreino, PCA.dimensaoPCA);
		for (int i = 0; i < SubtraiMedia.numExemplosTreino; i++) {
			Padrao temp = new Padrao(PCA.dimensaoPCA);
			for (int j = 0; j < PCA.dimensaoPCA; j++) {
				double att = arqBaseTreino.readDouble();
				temp.atributos.add(j, att);
				if(att > baseTreino.maxValues.get(j)){
					baseTreino.maxValues.set(j, att);
				}
				if(att < baseTreino.minValues.get(j)){
					baseTreino.minValues.set(j, att);
				}
			}
			temp.classe = arqBaseTreino.readString();
			baseTreino.padroes.add(temp);
		}
		arqBaseTreino.close();
		System.out.println("Base de treino carregada!");
	}
}
