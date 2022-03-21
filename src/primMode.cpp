#include <stdio.h>
#include <graphics.h>
#include <iostream>
#include <fstream>
#include <math.h>

// -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32
#define INF 9999
#define BLANK 0

const int WIDTH = 850;
const int HEIGHT = 750;
const float BOLDNESS = 3.5;
const float selectingRadius = 10;
const int DELAY_TIME = 75;
const char FILENAME[] = "input.txt";

void clearSet(int set[], int n) {
	for (int i = 0; i < n; ++i) {
		set[i] = BLANK;
	}
}

void fillSet(int set[], int n) {
	for (int i = 0; i < n; ++i) {
		set[i] = 1;
	}
}

void printSet(int set[], int n) {
	printf("{ ");
	for (int i = 0; i < n; ++i) {
		if (set[i] != BLANK) {
			printf("v%d, ", i + 1);
		}
	}

	printf(" }\n");
}

int getSize(int set[], int n) {
	int count = 0;
	for (int i = 0; i < n; ++i) {
		if (set[i] == 1) {
			count++;
		}
	}

	return count;
}

void updateTable(int *table, int *vertex_table, int v, int round, int *matrix, int n) {
	for (int i = 0; i < n; ++i) {
		if (i == v) {
			// (*table)[round][i] = BLANK;
			*((table + round *n) + i) = BLANK;
			continue;
		}

		if (*((matrix + v *n) + i) != 0) {
			*((table + round *n) + i) = *((matrix + v *n) + i);
			*((vertex_table + round *n) + i) = v;
		}

		if (round > 0) {
			if (*((table + (round - 1) *n) + i)<*((table + round *n) + i)) {
				*((table + round *n) + i) = *((table + (round - 1) *n) + i);
				*((vertex_table + round *n) + i) = *((vertex_table + (round - 1) *n) + i);
			}
		}
	}
}

void printTable(int *table, int *vertex_table, int round, int label[], int n) {
	// column header
	printf("========");
	for (int i = 0; i < n; ++i) {
		printf("========");
	}

	printf("\n| N \t|");
	for (int i = 0; i < n; ++i) {
		printf(" v%d\t|", i + 1);

	}
	printf("\n========");
	for (int i = 0; i < n; ++i) {
		printf("========");
	}

	printf("\n");

	for (int i = 0; i <= round; ++i) {
		// row header
		
		printf("| v%d\t|", label[i]);
		

		for (int j = 0; j < n; ++j) {
			// table's content
			if (*((table + i *n) + j) == INF) {
				printf(" inf   |");
			} else if (*((table + i *n) + j) == 0) {
				printf(" -     |");
			} else {
				// printf(" %d, v%d |", *((table + i *n) + j), *((vertex_table + i *n) + j) + 1);
				if ((*((table + i *n) + j)) + 1 > 9 && (*((vertex_table + i *n) + j)) + 1 > 9) {
					printf(" %d,v%d |", *((table + i *n) + j), *((vertex_table + i *n) + j) + 1);
				} else if ((*((table + i *n) + j)) + 1 > 9 && (*((vertex_table + i *n) + j)) + 1 <= 9) {
					printf(" %d,v%d |", *((table + i *n) + j), *((vertex_table + i *n) + j) + 1);
				} else if ((*((table + i *n) + j)) + 1 <= 9 && (*((vertex_table + i *n) + j)) + 1 > 9) {
					printf(" %d,v%d |", *((table + i *n) + j), *((vertex_table + i *n) + j) + 1);
				} else {
					printf(" %d,v%d  |", *((table + i *n) + j), *((vertex_table + i *n) + j) + 1);
				}
			}
		}

		printf("\n");
	}

	

	printf("\n");

}

void printPath(int T1[], int T2[], int round) {
	printf("  T = { ");
	for (int i = 0; i < round; ++i) {
		if (i == round - 1) {
			printf("v%dv%d ", T1[i], T2[i]);
			break;
		}

		printf("v%dv%d, ", T1[i], T2[i]);
	}

	printf("}\n");
}

void initializeTables(int *table, int *vertex_table, int n) {
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			*((table + i *n) + j) = INF;
			*((vertex_table + i *n) + j) = -1;
		}
	}
}

int getMin(int *table, int vertex_table[], int round, int n) {
	int min = INF;
	int i_min;
	for (int i = 0; i < n; ++i) {
		int table_i = *((table + round *n) + i);
		if (table_i < min && table_i != 0) {
			min = table_i;
			i_min = i;
		}
	}

	return i_min;
}

int getWeight(int T1[], int T2[], int *matrix, int n) {
	int weight = 0;
	for (int i = 0; i < n - 1; ++i) {
		int x = T1[i] - 1;
		int y = T2[i] - 1;
		weight += *((matrix + x *n) + y);
	}

	return weight;
}

int prim(int *matrix, int v, int n, int *pT1, int *pT2) {
	printf("\n\n");

	v--;
	int u = v;

	// initailizing
	int T1[n-1];
	clearSet(T1, n);
	int T2[n-1];
	clearSet(T2, n);
	int N[n];
	clearSet(N, n);
	N[v] = 1;
	int A[n];
	fillSet(A, n);
	A[v] = 0;
	int table[n][n];
	int vertex_table[n][n];
	initializeTables(*table, *vertex_table, n);
	int round = 0;
	int label[n];
	clearSet(label, n);

	// preparing
	label[round] = v + 1;
	printf("=== Prim's Algorithm starting at v%d ===\n", v + 1);
	updateTable(*table, *vertex_table, v, round, matrix, n);	// (int *table, int *vertex_table, int v, int round, int *matrix, int n)
	printTable(*table, *vertex_table, round, label, n);
	printPath(T1, T2, round);
	printf("  A = ");
	printSet(A, n);
	printf("  N = ");
	printSet(N, n);
	printf("\n\n");

	// algorithm
	while (round < n - 1) {
		printf("--> round : %d <--\n", round + 1);
		v = getMin(*table, vertex_table[round], round, n);
		label[round + 1] = v + 1;
		round++;
		updateTable(*table, *vertex_table, v, round, matrix, n);
		u = vertex_table[round - 1][v];
		N[v] = 1;
		A[v] = 0;
		T1[round - 1] = u + 1;
		T2[round - 1] = v + 1;
		*(pT1 + round - 1) = v + 1;
		*(pT2 + round - 1) = u + 1;

		printTable(*table, *vertex_table, round - 1, label, n);
		printPath(T1, T2, round);
		printf("  A = ");
		printSet(A, n);
		printf("  N = ");
		printSet(N, n);
		printf("\n\n");
	}


	int weight = getWeight(T1, T2, matrix, n);
	printf("### Total weight = %d ### \n\n", weight);


	return 0;

}

void printMatrix(int *matrix, int n) {
	printf("Graph Matrix = ");
	for (int i = 0; i < n; ++i) {
		if (i == 0) {
			printf("| ");
		} else {
			printf("               | ");
		}

		for (int j = 0; j < n; ++j) {
			printf("%d ", *((matrix + i *n) + j));
		}

		printf("|\n");
	}
}

void drawVertex(int x_vertex[], int y_vertex[], int n_i) {
	char label[5];
	for (int i = 0; i < n_i; ++i) {
		sprintf(label, "v%d", (i + 1));

		int xv = x_vertex[i];
		int yv = y_vertex[i];
		setcolor(15);
		fillellipse(xv, yv, BOLDNESS, BOLDNESS);
		setcolor(14);
		outtextxy(xv - 5, yv + 5, label);
		setcolor(15);

	}
}

void draw_edge_tag(int x, int y, int num) {
	char e_tag[3];
	sprintf(e_tag, "%d", num);
	setcolor(3);
	outtextxy(x, y, e_tag);
	setcolor(15);

}

void drawEdge(int x_vertex_position[], int y_vertex_position[], int *matrix, int n) {
	const int RADIUS = 150;
	int centerX = WIDTH / 2;
	int centerY = HEIGHT / 2;

	for (int i = 0; i < n; ++i) {
		for (int j = i; j < n; ++j) {
			if (*((matrix + i *n) + j) != 0) {
				int x = (x_vertex_position[i] + x_vertex_position[j]) / 2;
				int y = (y_vertex_position[i] + y_vertex_position[j]) / 2;
				line(x_vertex_position[i], y_vertex_position[i], x_vertex_position[j], y_vertex_position[j]);
				draw_edge_tag(x, y, *((matrix + i *n) + j));

			}
		}
	}
}

void drawPrim(int x_vertex_position[], int y_vertex_position[], int *pT1, int *pT2, int n) {
	for (int i = 0; i < n - 1; ++i) {
		int x1 = x_vertex_position[*(pT1 + i) - 1];
		int y1 = y_vertex_position[*(pT1 + i) - 1];
		int x2 = x_vertex_position[*(pT2 + i) - 1];
		int y2 = y_vertex_position[*(pT2 + i) - 1];
		line(x1, y1, x2, y2);
	}
}

int selectedVertexPosition(int x, int y, int x_vertex[], int y_vertex[], int n) {
	for (int i = 0; i < n; ++i) {
		bool x_condition = ((x < (x_vertex[i] + selectingRadius)) && (x >= (x_vertex[i] - selectingRadius)));
		bool y_condition = ((y < (y_vertex[i] + selectingRadius)) && (y >= (y_vertex[i] - selectingRadius)));

		if (x_condition && y_condition) {
			return i;
		}
	}

	return -1;

}

void drawButton(int left, int top, int right, int bottom, char text[]) {
	rectangle(left, top, right, bottom);
	int centerX = (right - left) / 2;
	int centerY = top + (bottom / 2);
	outtextxy(left + 10, top + 10, text);
}

bool notUILocation(int x, int y) {
	bool state = true;
	if (y > HEIGHT - 100) {
		return false;
	}

	if (y < 60) {
		return false;
	}

	return state;
}

void getMatrix(int x, int y, int *matrix, int n) {
	if (x > 10 && x < 150 && y > 10 && y < 50) {
		for (int i = 0; i < n; ++i) {
			printf("--");
		}

		printf("\n");
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				printf("%d ", *((matrix + i *n) + j));
			}

			printf("\n");
		}

		for (int i = 0; i < n; ++i) {
			printf("--");
		}

		printf("\n");
	}

	return;

}

int isDeleting(int x, int y, bool deleting) {
	if (x > 160 && x < 250 && y > 10 && y < 50 && deleting) {
		return 0;
	} else if (x > 160 && x < 250 && y > 10 && y < 50 && !deleting) {
		return 1;
	} else {
		return -1;
	}
}

bool clear(int x, int y, int *matrix, int n, int *n_i, int x_vertex[], int y_vertex[]) {
	if (x > 260 && x < 340 && y > 10 && y < 50) {
		for (int i = 0; i < n; ++i) {
			x_vertex[i] = 0;
			y_vertex[i] = 0;
			for (int j = 0; j < n; ++j) {
				*((matrix + i *n) + j) = 0;
			}
		}

		*n_i = 0;
		printf("cleared\n");
		return true;
	}

	return false;

}

void moreWeight(int x, int y, int *weight) {
	if (x > 450 && x < 480 && y > 10 && y < 50) {
		*weight += 1;
	}
}

void lessWeight(int x, int y, int *weight) {
	if (x > 350 && x < 380 && y > 10 && y < 50) {
		*weight -= 1;
	}
}

bool getPrim(int *matrix, int v, int n, int *pT1, int *pT2) {
	int connected = 0;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			if (*((matrix + i *n) + j) != 0) {
				connected++;
				break;
			}
		}
	}

	if (connected < n) {
		printf("!! not connected graph !!\n");
		return false;
	}

	prim(matrix, v, n, pT1, pT2);
	return true;

}

void draw(int *matrix, int n, int x_vertex_position[], int y_vertex_position[]) {
	const int boldness = 3;
	const int radius = 300;
	int centerX = WIDTH / 2;
	int centerY = HEIGHT / 2;

	// drawing vertexes and vertex labels
	for (int i = 0; i < n; ++i) {
		char label[3];
		sprintf(label, "v%d", i + 1);

		// https://bit.ly/3tLpBAi
		float angle = 2 * 3.14 / n * i;
		int x = ((radius* cos(angle)) + centerX);
		int y = ((radius* sin(angle)) + centerY);

		x_vertex_position[i] = x;
		y_vertex_position[i] = y;

		fillellipse(x, y, boldness, boldness);
		setcolor(15);
		settextstyle(3, 0, 1);
		setcolor(14);
		outtextxy(x - 5, y + 5, label);
		setcolor(15);
	}

	// drawing edges
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			if (*((matrix + i *n) + j) != 0 && i != j) {
				char edge_label[3];
				int centerX = (x_vertex_position[i] + x_vertex_position[j]) / 2;
				int centerY = (y_vertex_position[i] + y_vertex_position[j]) / 2;
				line(x_vertex_position[i], y_vertex_position[i], x_vertex_position[j], y_vertex_position[j]);
				sprintf(edge_label, "%d", *((matrix + i *n) + j));
				setcolor(3);
				outtextxy(centerX, centerY, edge_label);
				setcolor(15);
			}
		}
	}
}

int main() {
	int mode;
	printf("=========================================================================\n");

	printf("\n     .+------+     +------+     +------+     +------+     +------+.  \n");
	printf("   .' |      |    /|      |     |      |     |      |\\    |      | `.  \n");
	printf("  +   |      |   + |      |     +      +     |      | +   |      |   +  \n");
	printf("  |   |      |   | |      |     |      |     |      | |   |      |   |  \n");
	printf("  |  .+------+   | +------+     +------+     +------+ |   +------+.  |  \n");
	printf("  |.'      .'    |/      /      |      |      \\      \\|    `.      `.|  \n");
	printf("  +------+'      +------+       +------+       +------+      `+------+  \n\n");

	printf("=========================================================================\n");
	printf("--- Select Mode ---\n");
	printf(" - [1] Text File Input Mode (input.txt) ** Recommend for vertices below 10 vertices\n");
	printf(" - [2] Mouse Drawing Mode\n");
	printf(" - [3] Text File Input, draw vertex (input.txt) \n");

	printf("\n!!! Caution !!! - Too many vertices or edges may cause this program to flickering\n");
	printf("-------------------\n");
	printf("Enter Mode : ");

	while (true) {
		scanf("%d", &mode);
		printf("\n");
		if (mode == 1) {
			// Text Files Input Mode
			std::fstream getN(FILENAME, std::ios_base:: in);
			std::fstream file1(FILENAME, std::ios_base:: in);

			if (!getN.is_open()) {
				printf("Error!! >> cannot open text file! \n");
				getch();
				return -1;
			}

			int a;
			int k = 0;
			while (getN >> a) {
				k++;
			}

			int n = sqrt(k);
			int matrix[n][n];
			int v;
			int w;
			int in = 0;
			int l = 0;

			while (file1 >> a) {
				if (in == k - 2) {
					v = a;
				} else if (in == k - 1) {
					w = a;
				} else {
					*(*(matrix) + l) = a;
					l++;
				}
				in ++;
			}

			printf("\n>> Reading : n = %d, starting vertex = %d, total weight = %d\n", n, v, w);

			int x_vertex_position[n];
			int y_vertex_position[n];
			int pT1[n];
			clearSet(pT1, n);
			int pT2[n];
			clearSet(pT2, n);
			char modeInfo[35];
			sprintf(modeInfo, "     Drawing from input.txt - Prim starting at v%d ", v);
			char showPrimLabel[] = "Hide Prim";

			initwindow(WIDTH, HEIGHT, "Graph Drawer");
			bool connected = getPrim(*matrix, v, n, pT1, pT2);
			if (!connected) {
				return -1;
			}
			bool toDrawPrim = true;

			while (1) {
				cleardevice();

				drawButton(10, 10, 120, 50, showPrimLabel);
				if (ismouseclick(WM_RBUTTONDOWN)) {
					if(mousex() > 10 && mousex() < 120 && mousey() > 10 && mousey() < 50) {
						if (toDrawPrim) {
							sprintf(showPrimLabel, "Show Prim");
						} else {
							sprintf(showPrimLabel, "Hide Prim");
						}
						toDrawPrim = !toDrawPrim;
						clearmouseclick(WM_RBUTTONDOWN);
					}
				}

				outtextxy(0, HEIGHT-50, modeInfo);

				if (toDrawPrim) {
					setlinestyle(0, 1, 3);
					drawPrim(x_vertex_position, y_vertex_position, pT1, pT2, n);
				}
				setlinestyle(0, 1, 1);
				draw(*matrix, n, x_vertex_position, y_vertex_position);
				delay(DELAY_TIME*2);

			
				

			}
		} else if (mode == 2) {
			// Mouse Drawing Mode

			int n;
			printf("Number of vertex : ");
			scanf("%d", &n);
			int x_vertex[n];
			int y_vertex[n];
			int matrix[n][n];
			int n_i = 0;

			for (int i = 0; i < n; ++i) {
				for (int j = 0; j < n; ++j) {
					matrix[i][j] = 0;
				}
			}

			char title[] = "get matrix";

			// int initwindow(int width, int height, const char* title="Windows BGI", int left=0, int top=0, bool dbflag=false, closeflag=true);
			initwindow(WIDTH, HEIGHT, title, 100, 100, false, true);

			bool selected_vertex = false;

			int startVertex = -1;
			int endVertex = -1;

			char matrixLabel[] = "get matrix";
			char deleteLabel[] = "delete";
			char clearLabel[] = "clear";
			char plusWeight[] = "+";
			char minusWeight[] = "-";
			char weightLabel[10] = "0";
			char primLabel[] = "Prim";

			char state_label[25] = "drawing vertex";
			char instructor_label[50] = "- left click to draw vertex";
			char instructor_label1[50] = "";

			bool deleting = false;

			int weight = 1;
			int pT1[n];
			clearSet(pT1, n);
			int pT2[n];
			clearSet(pT2, n);

			bool selectedPrimVertex = false;
			bool clickedPrimButton = false;
			bool toDrawPrim = false;
			int v = 1;

			while (true) {
				cleardevice();

				settextstyle(8, 0, 1);
				sprintf(weightLabel, "%d", weight);

				if (ismouseclick(WM_RBUTTONDOWN)) {
					// right clicking button
					int rClickX = mousex();
					int rClickY = mousey();

					if (rClickX > 490 && rClickX < 560 && rClickY > 10 && rClickY < 50) {
						clickedPrimButton = true;
					}

					int deleteState = isDeleting(rClickX, rClickY, deleting);
					if (deleteState != -1) {
						deleting = !deleting;
					}

					getMatrix(rClickX, rClickY, *matrix, n);
					if (clear(rClickX, rClickY, *matrix, n, &n_i, x_vertex, y_vertex)) {
						toDrawPrim = false;
					}

					if (weight >= 0) {
						moreWeight(rClickX, rClickY, &weight);
					}

					if (weight > 0) {
						lessWeight(rClickX, rClickY, &weight);
					}

					clearmouseclick(WM_RBUTTONDOWN);
				}

				if (n_i < n) {
					// drawing vertices
					sprintf(state_label, "drawing vertex");
					sprintf(instructor_label, "- left click to draw vertex");
					sprintf(instructor_label1, "");

					if (ismouseclick(WM_LBUTTONDOWN)) {
						int selecting_x = mousex();
						int selecting_y = mousey();

						if (notUILocation(selecting_x, selecting_y)) {
							x_vertex[n_i] = selecting_x;
							y_vertex[n_i] = selecting_y;
							n_i++;
						}

						clearmouseclick(WM_LBUTTONDOWN);
					}
				}



				if (clickedPrimButton) {
					sprintf(state_label, "Prim's Algorithm");
					sprintf(instructor_label, "- select start vertex");
					sprintf(instructor_label1, "");

					if (ismouseclick(WM_LBUTTONDOWN)) {
						int selecting_x = mousex();
						int selecting_y = mousey();
						if (selectedVertexPosition(selecting_x, selecting_y, x_vertex, y_vertex, n) != -1 && notUILocation(selecting_x, selecting_y)) {
							v = selectedVertexPosition(selecting_x, selecting_y, x_vertex, y_vertex, n) + 1;
							selectedPrimVertex = true;
							clickedPrimButton = false;
						}

						clearmouseclick(WM_LBUTTONDOWN);
					}
				}

				if (n_i >= n && !clickedPrimButton) {
					// vertex is drawn
					drawButton(160, 10, 250, 50, deleteLabel);
					drawButton(490, 10, 560, 50, primLabel);	// prim button
					drawButton(260, 10, 340, 50, clearLabel);	// clear button

					if (deleting && !clickedPrimButton) {
						sprintf(deleteLabel, " draw");
						sprintf(state_label, "deleting edge");
						sprintf(instructor_label, "- left click at vertex to select start vertex to delete");
						sprintf(instructor_label1, "");
					} else if (!deleting && !clickedPrimButton) {
						sprintf(deleteLabel, "delete");
						sprintf(state_label, "drawing edge");
						sprintf(instructor_label, "- left click at vertex to select start vertex to draw");
						sprintf(instructor_label1, "");
					}

					if (ismouseclick(WM_LBUTTONDOWN)) {
						toDrawPrim = false;
						setlinestyle(0, 1, 1);
						int selecting_x = mousex();
						int selecting_y = mousey();
						if (selectedVertexPosition(selecting_x, selecting_y, x_vertex, y_vertex, n) != -1 && notUILocation(selecting_x, selecting_y)) {
							if (selected_vertex) {
								selected_vertex = false;
								endVertex = selectedVertexPosition(selecting_x, selecting_y, x_vertex, y_vertex, n);
								if (deleting) {
									// printf("deleting edge v%d to v%d\n", startVertex+1, endVertex+1);
									matrix[startVertex][endVertex] = 0;
									matrix[endVertex][startVertex] = 0;
								} else if (!deleting && startVertex != endVertex) {
									// printf("draw edge from v%d to v%d with weight = %d \n", startVertex+1, endVertex+1, weight);
									matrix[startVertex][endVertex] = weight;
									matrix[endVertex][startVertex] = weight;
								}
							} else {
								selected_vertex = true;
								startVertex = selectedVertexPosition(selecting_x, selecting_y, x_vertex, y_vertex, n);
							}
						}

						clearmouseclick(WM_LBUTTONDOWN);
					}
				}

				if (selected_vertex) {
					//
					toDrawPrim = false;
					if (ismouseclick(WM_LBUTTONDBLCLK)) {
						selected_vertex = false;
						clearmouseclick(WM_LBUTTONDBLCLK);
					}

					sprintf(instructor_label, "- left click at vertex to select end vertex");
					sprintf(instructor_label1, "- double click to discard");
					int y = mousey();
					if (y < 60) {
						y = 60;
					}

					if (y > HEIGHT - 110) {
						y = HEIGHT - 110;
					}

					line(x_vertex[startVertex], y_vertex[startVertex], mousex(), y);
				}

				line(0, 60, WIDTH, 60);
				line(0, HEIGHT - 110, WIDTH, HEIGHT - 110);
				outtextxy(10, HEIGHT - 100, state_label);
				outtextxy(10, HEIGHT - 75, instructor_label);
				outtextxy(10, HEIGHT - 50, instructor_label1);

				if (selectedPrimVertex) {
					toDrawPrim = getPrim(*matrix, v, n, pT1, pT2);
					selectedPrimVertex = false;
				}

				if (toDrawPrim) {
					setlinestyle(0, 1, 3);
					drawPrim(x_vertex, y_vertex, pT1, pT2, n);
				}

				setlinestyle(0, 1, 1);
				drawEdge(x_vertex, y_vertex, *matrix, n);
				drawVertex(x_vertex, y_vertex, n_i);

				drawButton(10, 10, 150, 50, matrixLabel);	// get matrix button
				drawButton(350, 10, 380, 50, minusWeight);	// minus weight button
				outtextxy(410, 20, weightLabel);	// weight Label
				drawButton(450, 10, 480, 50, plusWeight);	// plus weight button

				delay(DELAY_TIME);
			
			}
		}

		else if (mode == 3) {
			// Text Files Input Mode
			std::fstream getN(FILENAME, std::ios_base:: in);
			std::fstream file1(FILENAME, std::ios_base:: in);

			if (!getN.is_open()) {
				printf("Error!! >> cannot open text file! \n");
				getch();
				return -1;
			}

			int a;
			int k = 0;
			while (getN >> a) {
				k++;
			}

			int n = sqrt(k);
			int matrix[n][n];
			int v;
			int w;
			int in = 0;
			int l = 0;

			while (file1 >> a) {
				if (in == k - 2) {
					v = a;
				} else if (in == k - 1) {
					w = a;
				} else {
					*(*(matrix) + l) = a;
					l++;
				}
				in ++;
			}

			printf("\n>> Reading : n = %d, starting vertex = %d, total weight = %d\n", n, v, w);

			int x_vertex_position[n];
			int y_vertex_position[n];
			int pT1[n];
			clearSet(pT1, n);
			int pT2[n];
			clearSet(pT2, n);
			char modeInfo[35];
			sprintf(modeInfo, "     Drawing from input.txt - Prim starting at v%d ", v);
			char showPrimLabel[] = "Hide Prim";

			initwindow(WIDTH, HEIGHT, "Graph Drawer");
			bool connected = getPrim(*matrix, v, n, pT1, pT2);
			if (!connected) {
				return -1;
			}
			bool toDrawPrim = true;

			int i_n = 0;
			char state_label[] = "left click to draw vertex";
			char reDraw[] = "Re-Draw"; 

			settextstyle(8, 0, 1);

			while (1) {

				cleardevice();
				
				line(0, 60, WIDTH, 60);

				if (i_n < n) {
					if (ismouseclick(WM_LBUTTONDOWN) && mousey() > 60) {
						x_vertex_position[i_n] = mousex();
						y_vertex_position[i_n] = mousey();
						i_n++;
						clearmouseclick(WM_LBUTTONDOWN);
					}

					sprintf(state_label, "left click to draw vertex");
				} else {
					
					drawButton(120, 10, 250, 50, showPrimLabel);
					if (ismouseclick(WM_RBUTTONDOWN)) {
						int mx = mousex();
						int my = mousey();

						if (mx > 120 && mx < 250 && my > 10 && my< 50) {
							if (toDrawPrim) {
								sprintf(showPrimLabel, "Show Prim");
							} else {
								sprintf(showPrimLabel, "Hide Prim");
							}
							toDrawPrim = !toDrawPrim;
							clearmouseclick(WM_RBUTTONDOWN);
						}
						
					}

					if (toDrawPrim) {
						setlinestyle(0, 1, 3);
						drawPrim(x_vertex_position, y_vertex_position, pT1, pT2, n);
					}

					setlinestyle(0, 1, 1);
					sprintf(state_label, "");
					drawEdge(x_vertex_position, y_vertex_position, *matrix, n);
				}

				drawButton(10, 10, 110, 50, reDraw);
				if (ismouseclick(WM_RBUTTONDOWN)) {
					if (mousex() > 10 && mousex() < 110 && mousey() > 10 && mousey() < 50) {
							i_n = 0;
							
							clearmouseclick(WM_LBUTTONDOWN);
					}
					clearmouseclick(WM_RBUTTONDOWN);
				}

				

				drawVertex(x_vertex_position, y_vertex_position, i_n);
				

				outtextxy(0, HEIGHT-50, state_label);

				
				setlinestyle(0, 1, 1);
				delay(DELAY_TIME*2);
				
			}

			
		}
		else {
			printf("Error >> This mode doesn't exist!! Try Again\nEnter Mode : ");
		}


	}
		return 0;
}
