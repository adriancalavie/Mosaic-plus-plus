#pragma once
#include <vector>
#include "../Picture Tools/PictureTools.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <cmath>


class QuadTreeImages
{
	using pt = PictureTools;


	/*class QTree() :
		def __init__(self, stdThreshold, minPixelSize, img) :
		self.threshold = stdThreshold
		self.min_size = minPixelSize
		self.minPixelSize = minPixelSize
		self.img = img
		self.root = Node(0, 0, img.shape[0], img.shape[1])

		def get_points(self) :
		return img[self.root.x0:self.root.x0 + self.root.get_width(), self.root.y0 : self.root.y0 + self.root.get_height()]

		def subdivide(self) :
		recursive_subdivide(self.root, self.threshold, self.minPixelSize, self.img)

		def graph_tree(self) :
		fig = plt.figure(figsize = (10, 10))
		plt.title("Quadtree")
		c = find_children(self.root)
		print("Number of segments: %d" % len(c))
		for n in c :
	plt.gcf().gca().add_patch(patches.Rectangle((n.y0, n.x0), n.height, n.width, fill = False))
		plt.gcf().gca().set_xlim(0, img.shape[1])
		plt.gcf().gca().set_ylim(img.shape[0], 0)
		plt.axis('equal')
		plt.show()
		return

		def render_img(self, thickness = 1, color = (0, 0, 255)) :
		imgc = self.img.copy()
		c = find_children(self.root)
		for n in c :
	pixels = n.get_points(self.img)
		# grb
		gAvg = math.floor(np.mean(pixels[:, : , 0]))
		rAvg = math.floor(np.mean(pixels[:, : , 1]))
		bAvg = math.floor(np.mean(pixels[:, : , 2]))

		imgc[n.x0:n.x0 + n.get_width(), n.y0 : n.y0 + n.get_height(), 0] = gAvg
		imgc[n.x0:n.x0 + n.get_width(), n.y0 : n.y0 + n.get_height(), 1] = rAvg
		imgc[n.x0:n.x0 + n.get_width(), n.y0 : n.y0 + n.get_height(), 2] = bAvg

		if thickness > 0:
	for n in c :
	# Draw a rectangle
		imgc = cv2.rectangle(imgc, (n.y0, n.x0), (n.y0 + n.get_height(), n.x0 + n.get_width()), color, thickness)
		return imgc*/





		//FLORIN

		/*def recursive_subdivide(node, k, minPixelSize, img) :

			if node.get_error(img) <= k :
				return
				w_1 = int(math.floor(node.width / 2))
				w_2 = int(math.ceil(node.width / 2))
				h_1 = int(math.floor(node.height / 2))
				h_2 = int(math.ceil(node.height / 2))


				if w_1 <= minPixelSize or h_1 <= minPixelSize:
		return
			x1 = Node(node.x0, node.y0, w_1, h_1) # top left
			recursive_subdivide(x1, k, minPixelSize, img)

			x2 = Node(node.x0, node.y0 + h_1, w_1, h_2) # btm left
			recursive_subdivide(x2, k, minPixelSize, img)

			x3 = Node(node.x0 + w_1, node.y0, w_2, h_1)# top right
			recursive_subdivide(x3, k, minPixelSize, img)

			x4 = Node(node.x0 + w_1, node.y0 + h_1, w_2, h_2) # btm right
			recursive_subdivide(x4, k, minPixelSize, img)

			node.children = [x1, x2, x3, x4]*/

public:
	class QNode
	{
	public:

		int x0;
		int y0;
		int width;
		int height;
		pt::Point first;
		pt::Point second;
		cv::Scalar mean;
		std::vector<QNode*> children;
		cv::Mat points;
	public:
		QNode(int x0, int y0, int width, int height);


	public:

		int getWidth();
		int getHeigth();

		cv::Mat getPoints();
		cv::Mat getPoints(const cv::Mat&);
	public:
		double getError(const cv::Mat&);
	};

private:
	double threshold;
	int minSize;
	int minPixelSize;
	cv::Mat image;
	QNode* root;

	void recursiveSubDivide(QNode* node, double k, int minPixelSize, const cv::Mat& img);
	std::vector<QNode*> findChildren(QNode*);
public:
	void graphTree();

public:
	QuadTreeImages(double stdThreshold, int minPixelSize, const cv::Mat& image);
	cv::Mat renderImg(double thickness = 1, cv::Scalar color = cv::Scalar(0, 0, 255));
	void subdivide();
	cv::Mat concat_images(const cv::Mat& img1, const cv::Mat& img2, int boarder = 5, const cv::Scalar& color = (255, 255, 255));
};

