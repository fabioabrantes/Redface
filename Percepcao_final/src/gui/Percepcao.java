package gui;

import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.filechooser.FileFilter;

import preProcess.PCA.PCA;
import preProcess.leitura.PhotoToVector;
import preProcess.leitura.SubtraiMedia;
import util.Util_Imagem;
import Jama.Matrix;
import classificacao.basico.Padrao;
import classificacao.knn.KNN;
import classificacao.knn.TesteCompleto;

public class Percepcao extends JFrame {
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	private JPanel jContentPane = null;
	
	private JLabel jLabelTitulo = null;
	
	private JButton jButtonRodar = null;
	
	private JRadioButton jRadioButtonConjTeste = null;
	
	private JRadioButton jRadioButtonArquivo = null;
	
	/**
	 * This is the default constructor
	 * @throws IOException 
	 */
	public Percepcao(int resize, int pca) throws IOException {
		super();
		initialize();
		PhotoToVector.run(resize);
		SubtraiMedia.run();
		PCA.run(pca);
	}
	
	/**
	 * This method initializes this
	 * 
	 * @return void
	 */
	private void initialize() {
		this.setSize(300, 200);
		this.setContentPane(getJContentPane());
		this.setTitle("Percepcao");
		this.addWindowListener(new java.awt.event.WindowAdapter() {
			public void windowClosing(java.awt.event.WindowEvent e) {
				System.exit(0);
			}
		});
	}
	
	/**
	 * This method initializes jContentPane
	 * 
	 * @return javax.swing.JPanel
	 */
	private JPanel getJContentPane() {
		if (jContentPane == null) {
			jLabelTitulo = new JLabel();
			jLabelTitulo.setBounds(new java.awt.Rectangle(70,10,160,25));
			jLabelTitulo.setHorizontalAlignment(javax.swing.SwingConstants.CENTER);
			jLabelTitulo.setHorizontalTextPosition(javax.swing.SwingConstants.CENTER);
			jLabelTitulo.setText("Percepção computacional");
			jContentPane = new JPanel();
			jContentPane.setLayout(null);
			jContentPane.add(jLabelTitulo, null);
			jContentPane.add(getJButtonRodar(), null);
			jContentPane.add(getJRadioButtonConjTeste(), null);
			jContentPane.add(getJRadioButtonArquivo(), null);
		}
		return jContentPane;
	}
	
	/**
	 * This method initializes jButtonRodar	
	 * 	
	 * @return javax.swing.JButton	
	 */
	private JButton getJButtonRodar() {
		if (jButtonRodar == null) {
			jButtonRodar = new JButton();
			jButtonRodar.setBounds(new java.awt.Rectangle(110,115,80,25));
			jButtonRodar.setText("Rodar");
			jButtonRodar.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					if(jRadioButtonConjTeste.isSelected()){
						System.out.println("Conjunto de testes!");
						String acerto = TesteCompleto.run(1, 1, true);
						JOptionPane.showMessageDialog(null, acerto, "Taxa de acerto", JOptionPane.INFORMATION_MESSAGE);
					}else{
						System.out.println("Arquivo de imagem!");
						try{
							JFileChooser chooser = new JFileChooser("outputFaces120x90_separada/");
							chooser.setFileFilter(new FileFilter(){
								public boolean accept(File f) {
									return f.isDirectory() || f.getName().endsWith("jpg");
								}
								public String getDescription() {
									return "Arquivos JPG";
								}			
							});
							chooser.showOpenDialog(null);
							
							File selectedPhoto = chooser.getSelectedFile();
							String resizePath = selectedPhoto.getAbsolutePath().replaceAll(".jpg", "_resized.jpg");
							File resized = new File(resizePath);
							Util_Imagem.resize(selectedPhoto, resized, PhotoToVector.resize, 1f);
							BufferedImage image = ImageIO.read(resized);
							int[] pixels = Util_Imagem.pegarPixels(image);
							
							double[] pixelsDouble = new double[pixels.length];
							for (int i = 0; i < pixels.length; i++) {
								pixelsDouble[i] = (double)pixels[i];
							}
							
							String rotulo = JOptionPane.showInputDialog(null, "Qual a pasta da imagem carregada?");
							
							Matrix pattern = new Matrix(1, pixels.length);
							TesteCompleto.carregaMedias();
							for (int j = 0; j < pixelsDouble.length; j++) {
								double carac = pixelsDouble[j] - TesteCompleto.medias.get(j);
								pattern.set(0, j, carac);
							}
							TesteCompleto.carregaAutoVetores();
							pattern = pattern.times(TesteCompleto.autoVetores);
							double[] array = pattern.getArray()[0];
							Padrao novoPadraoTeste = new Padrao(PCA.dimensaoPCA);
							for (int j = 0; j < PCA.dimensaoPCA; j++) {
								novoPadraoTeste.atributos.add(j, array[j]);
							}
							TesteCompleto.carregaBaseTreino();
							String classificacao = KNN.k_nn(1, novoPadraoTeste, TesteCompleto.baseTreino, true);
							
							File classificado = new File("outputFaces120x90_separada/" + classificacao + "/_3.jpg");
							BufferedImage imageClassificado = ImageIO.read(classificado);
							ImageIcon icone = new ImageIcon(imageClassificado);
							
							if(rotulo.equalsIgnoreCase(classificacao)){
								JOptionPane.showMessageDialog(null, classificacao, "Classificação", JOptionPane.INFORMATION_MESSAGE, icone);
							}else{
								JOptionPane.showMessageDialog(null, "A imagem não pertence à base!", "Verificação", JOptionPane.ERROR_MESSAGE);
								//JOptionPane.showMessageDialog(null, classificacao, "Classificação incorreta!", JOptionPane.INFORMATION_MESSAGE, icone);
							}
						}catch(IOException exc){
							exc.printStackTrace();
						}
					}
				}
			});
		}
		return jButtonRodar;
	}
	
	/**
	 * This method initializes jRadioButtonConjTeste	
	 * 	
	 * @return javax.swing.JRadioButton	
	 */
	private JRadioButton getJRadioButtonConjTeste() {
		if (jRadioButtonConjTeste == null) {
			jRadioButtonConjTeste = new JRadioButton();
			jRadioButtonConjTeste.setBounds(new java.awt.Rectangle(80,55,140,20));
			jRadioButtonConjTeste.setText("Conjunto de testes");
			jRadioButtonConjTeste.setSelected(true);
			jRadioButtonConjTeste.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					if (!jRadioButtonConjTeste.isSelected()) {
						jRadioButtonConjTeste.setSelected(true);
					}else{
						jRadioButtonArquivo.setSelected(false);
					}
				}
			});
		}
		return jRadioButtonConjTeste;
	}
	
	/**
	 * This method initializes jRadioButtonArquivo	
	 * 	
	 * @return javax.swing.JRadioButton	
	 */
	private JRadioButton getJRadioButtonArquivo() {
		if (jRadioButtonArquivo == null) {
			jRadioButtonArquivo = new JRadioButton();
			jRadioButtonArquivo.setBounds(new java.awt.Rectangle(80,80,140,20));
			jRadioButtonArquivo.setText("Arquivo de imagem");
			jRadioButtonArquivo.addActionListener(new java.awt.event.ActionListener() {
				public void actionPerformed(java.awt.event.ActionEvent e) {
					if (!jRadioButtonArquivo.isSelected()) {
						jRadioButtonArquivo.setSelected(true);
					}else{
						jRadioButtonConjTeste.setSelected(false);
					}
				}
			});
		}
		return jRadioButtonArquivo;
	}
	
}
