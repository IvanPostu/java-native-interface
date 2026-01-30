package test1java;

import java.io.File;
import java.net.URISyntaxException;

public class JarUtils {

	private JarUtils() {

	}

	public static boolean isJar() {
		try {
			String path = TestNative.class.getProtectionDomain().getCodeSource().getLocation().toURI().getPath();

			// Check if the path ends with ".jar"
			File file = new File(path);
			return file.isFile() && path.endsWith(".jar");
		} catch (URISyntaxException e) {
			// In case of an unexpected URI issue
			e.printStackTrace();
			return false;
		}
	}

}
