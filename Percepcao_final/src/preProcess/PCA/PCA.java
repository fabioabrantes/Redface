package preProcess.PCA;

import java.io.IOException;

import preProcess.leitura.SubtraiMedia;
import Jama.EigenvalueDecomposition;
import Jama.Matrix;
import util.Arquivo;

public class PCA {

	public static int dimensaoPCA;
	public static String[] classe = new String[SubtraiMedia.numExemplosTreino];
	
	private static void printPCA(Matrix baseTransformada, Matrix autoVetores) {
		System.out.println("Imprimindo resultado PCA!");
		Arquivo arqAutoVetores = new Arquivo("fool.txt", "autoVetores.txt");
		for(int i = 0; i < autoVetores.getRowDimension(); i++){
			for(int j = 0; j < autoVetores.getColumnDimension(); j++)
				arqAutoVetores.print(autoVetores.get(i, j) + " ");
			arqAutoVetores.println();
		}
		arqAutoVetores.close();
		Arquivo arqBaseTranformada = new Arquivo("fool.txt", "treinoFinal.txt");
		for (int i = 0; i < SubtraiMedia.numExemplosTreino; i++) {
			for(int j = 0; j < dimensaoPCA; j++){
				arqBaseTranformada.print(baseTransformada.get(i,j) + " ");
			}
			arqBaseTranformada.println(classe[i]);
		}
		arqBaseTranformada.close();
	}
	
	public static void run(int dimensaoBase) throws IOException {
		dimensaoPCA = dimensaoBase;
		
		Arquivo arqBase = new Arquivo("treinoMenosMedia.txt", "fool.txt");
		System.out.println("Lendo base...");
		
		Matrix T = new Matrix(SubtraiMedia.numExemplosTreino, SubtraiMedia.numAtributos);
		
		for (int i = 0; i < SubtraiMedia.numExemplosTreino; i++) {
			for (int j = 0; j < SubtraiMedia.numAtributos; j++) {
				T.set(i, j, arqBase.readDouble());
			}
			classe[i] = arqBase.readString();
		}
		arqBase.close();
		System.out.println("Base carregada!");
		
		T = T.transpose();
		
		Matrix S = T.transpose().times(T);
		System.out.println("Matriz de covariância (S) calculada!");
		
		EigenvalueDecomposition Svi = S.eig();
		Matrix TTtui = Svi.getV();
		double[] autoValores = Svi.getRealEigenvalues();
		
		for (int i = 0; i < autoValores.length; i++) {
			autoValores[i] = Math.sqrt(Math.abs(autoValores[i]));
		}
		System.out.println("Auto-valores calculados!");
		
		Matrix E = new Matrix(autoValores.length, autoValores.length);
		for (int i = 0; i < autoValores.length; i++) {
			for (int j = 0; j < autoValores.length; j++) {
				if(i==j)E.set(i, j, autoValores[i]);
			}
		}
		
		Matrix autoVetores = T.times(TTtui).times(E.inverse());
		
		int[] colunas = new int[dimensaoPCA];
		for (int i = 0; i < colunas.length; i++) {
			colunas[i] = SubtraiMedia.numExemplosTreino - i - 1;
		}
		autoVetores = autoVetores.getMatrix(0, autoVetores.getRowDimension() - 1, colunas);
		System.out.println("Auto-vetores calculados!");
		printPCA((autoVetores.transpose().times(T)).transpose(), autoVetores);
		
		System.out.println("PCA terminado!");
	}
}
