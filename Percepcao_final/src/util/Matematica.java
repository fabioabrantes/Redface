package util;

import java.util.Vector;

import classificacao.basico.Padrao;

public class Matematica {

	public static double euclidianaNormalizada(Padrao a, Padrao b, Vector<Double> max, Vector<Double> min){
		double somatorio = 0.0;
		for (int i = 0; i < a.atributos.size(); i++) {
			somatorio += dNorm(a.atributos.elementAt(i), 
					b.atributos.elementAt(i), 
					max.elementAt(i),
					min.elementAt(i));
		}
		return Math.sqrt(somatorio);
	}
	
	private static double dNorm(double a, double b, double max, double min){
		return Math.pow((a - b)/(max - min), 2.0);
	}
	
	public static double euclidiana(Padrao a, Padrao b){
		double somatorio = 0.0;
		for (int i = 0; i < a.atributos.size(); i++) {
			somatorio += Math.pow(a.atributos.elementAt(i) - b.atributos.elementAt(i), 2.0);
		}
		return Math.sqrt(somatorio);
	}
	
	public static double manhattan(Padrao a, Padrao b){
		double somatorio = 0.0;
		for (int i = 0; i < a.atributos.size(); i++) {
			somatorio = somatorio + Math.abs(a.atributos.elementAt(i) - b.atributos.elementAt(i));
		}
		return somatorio;
	}
	
}
