class SymetricMatrix { 

	public:

	// Constructor
	
	SymetricMatrix(double c=0) { loopi(0,10) m[i] = c;  }

	//4*4对称矩阵的上三角
	SymetricMatrix(	double m11, double m12, double m13, double m14, 
			            double m22, double m23, double m24,
			                        double m33, double m34,
			                                    double m44) {
			 m[0] = m11;  m[1] = m12;  m[2] = m13;  m[3] = m14; 
			              m[4] = m22;  m[5] = m23;  m[6] = m24; 
			                           m[7] = m33;  m[8] = m34;
			                                        m[9] = m44;
	}

	// Make plane
	//列向量与行向量相称(a,b,c,d是平面方程的系数:ax+by+cz+d=0)
	SymetricMatrix(double a,double b,double c,double d)
	{
		m[0] = a*a;  m[1] = a*b;  m[2] = a*c;  m[3] = a*d; 
		             m[4] = b*b;  m[5] = b*c;  m[6] = b*d; 
		                          m[7 ] =c*c; m[8 ] = c*d;
		                                       m[9 ] = d*d;
	}
	
	double operator[](int c) const { return m[c]; }

	// Determinant
	//3*3矩阵行列式(主对角线乘积之和减去副对角线乘积之和)
	double det(	int a11, int a12, int a13,
				int a21, int a22, int a23,
				int a31, int a32, int a33)
	{
		double det =  m[a11]*m[a22]*m[a33] + m[a13]*m[a21]*m[a32] + m[a12]*m[a23]*m[a31] 
					- m[a13]*m[a22]*m[a31] - m[a11]*m[a23]*m[a32]- m[a12]*m[a21]*m[a33]; 
		return det;
	}

	//矩阵相加
	const SymetricMatrix operator+(const SymetricMatrix& n) const
	{ 
		return SymetricMatrix( m[0]+n[0],   m[1]+n[1],   m[2]+n[2],   m[3]+n[3], 
						                    m[4]+n[4],   m[5]+n[5],   m[6]+n[6], 
						                                 m[ 7]+n[ 7], m[ 8]+n[8 ],
						                                              m[ 9]+n[9 ]);
	}

	//矩阵相加
	SymetricMatrix& operator+=(const SymetricMatrix& n)
	{
		 m[0]+=n[0];   m[1]+=n[1];   m[2]+=n[2];   m[3]+=n[3]; 
		 m[4]+=n[4];   m[5]+=n[5];   m[6]+=n[6];   m[7]+=n[7]; 
		 m[8]+=n[8];   m[9]+=n[9];
		return *this; 
	}

	double m[10];
};
///////////////////////////////////////////

namespace Simplify
{
	// Global Variables & Strctures

	struct Triangle { int v[3];double err[4];int deleted,dirty;vec3f n; };
	struct Vertex { vec3f p;int tstart,tcount;SymetricMatrix q;int border;};
	struct Ref { int tid,tvertex; }; 
	std::vector<Triangle> triangles;
	std::vector<Vertex> vertices;
	std::vector<Ref> refs;
	
	// Helper functions

	double vertex_error(SymetricMatrix q, double x, double y, double z);
	double calculate_error(int id_v1, int id_v2, vec3f &p_result);
	bool flipped(vec3f p,int i0,int i1,Vertex &v0,Vertex &v1,std::vector<int> &deleted);
	void update_triangles(int i0,Vertex &v,const std::vector<int> &deleted,int &deleted_triangles);
	void update_mesh(int iteration);
	void compact_mesh();
	//
	// Main simplification function 
	//
	// target_count  : target nr. of triangles
	// agressiveness : sharpness to increase the threashold.
	//                 5..8 are good numbers
	//                 more iterations yield higher quality
	//
	void simplify_mesh(int target_count, double agressiveness=7)
	{
		// init
		printf("%s - start\n",__FUNCTION__);
		int timeStart=timeGetTime();

		loopi(0,triangles.size()) triangles[i].deleted=0;  //删除标记
		
		// main iteration loop 

		int deleted_triangles=0; 
		std::vector<int> deleted0,deleted1;
		int triangle_count=triangles.size();
		
		loop(iteration,0,100) 
		{
			// target number of triangles reached ? Then break
			printf("iteration %d - triangles %d\n",iteration,triangle_count-deleted_triangles);
			if(triangle_count-deleted_triangles<=target_count)break;

			// update mesh once in a while
			if(iteration%5==0) 
			{
				/*
				1)删除delete的三角形(每次调用)
				2)初始化边的能量值(初次调用)
				3)重新计算拓扑连接关系(每次调用)
				4)收集边界点信息(初次调用)
				*/
				update_mesh(iteration);
			}

			// clear dirty flag
			loopi(0,triangles.size()) triangles[i].dirty=0;  //是否需要重新计算能量值的标记
			
			//
			// All triangles with edges below the threshold will be removed
			//
			// The following numbers works well for most models.
			// If it does not, try to adjust the 3 parameters
			//
			double threshold = 0.000000001*pow(double(iteration+3),agressiveness);

			// remove vertices & mark deleted triangles			
			loopi(0,triangles.size())
			{				
				Triangle &t=triangles[i];
				if(t.err[3]>threshold) continue;  //三条边中能量值最小的已经大于阈值
				if(t.deleted) continue;  //包含待塌边的三角形标记为删除状态
				if(t.dirty) continue;    //仅仅包含待塌边的其中一个顶点的三角形标记为置脏(需要重新计算能量值)状态
				
				loopj(0,3)if(t.err[j]<threshold) 
				{
					int i0=t.v[ j     ]; Vertex &v0 = vertices[i0]; 
					int i1=t.v[(j+1)%3]; Vertex &v1 = vertices[i1];

					// Border check
					if(v0.border != v1.border)  continue;  //边的两个端点中有一个点只与一个三角形连接

					// Compute vertex to collapse to
					vec3f p;
					calculate_error(i0,i1,p);  //计算最优塌边点

					deleted0.resize(v0.tcount); // normals temporarily v0连接的所有三角形
					deleted1.resize(v1.tcount); // normals temporarily v1连接的所有三角形

					/*
					1)检测将要变化(delta)的三角形用点p代替其中的一个端点是否会发生反转(法向偏转超过78.5度);
					2)收集在缩边过程中与要缩的边连接的三角形(dead),在deleted0,deleted1对应的位置做标记1;
					*/
					if( flipped(p,i0,i1,v0,v1,deleted0) ) continue;
					if( flipped(p,i1,i0,v1,v0,deleted1) ) continue;

					// not flipped, so remove edge
					/*
					1)删除dead三角形(与边{i0,i1}连接的所有三角形);
					2)消失一个顶点vertex(v1);
					*/
					v0.p=p;  //保留v0,用最优化点(二次误差最小点)更新v0的几何位置;
					v0.q=v1.q+v0.q; //将v1的二次误差矩阵(平面方程)累加到v0上;
					int tstart=refs.size();

					/*
					1)将delta三角形的v0或者v1顶点在各自三角形中的角色替换成i0;
					2)重新计算delta三角形各边的二次误差及其最优点;
					3)将保留的corner对象追加到refs.back();
					*/
					update_triangles(i0,v0,deleted0,deleted_triangles);
					update_triangles(i0,v1,deleted1,deleted_triangles);
					
					//本次缩边产生的新的corner数量
					int tcount=refs.size()-tstart;
				
					//v0的corner空间足够容纳新corner对象,则直接拷贝
					if(tcount<=v0.tcount)
					{
						// save ram
						if(tcount)memcpy(&refs[v0.tstart],&refs[tstart],tcount*sizeof(Ref));
					}
					//v0的corner空间不足,则重置v0的corner空间的v0.tstart,指向新的位置
					else
						// append
						v0.tstart=tstart;

					//重新制定v0的corner对象的数目
					v0.tcount=tcount;
					break;
				}
				// done?
				//缩边终止判断
				if(triangle_count-deleted_triangles<=target_count)break;
			}
		}

		// clean up mesh
		//删除废弃的三角形和顶点对象,但不确保corner信息完整
		compact_mesh();

		// ready
		//迭代完成
		int timeEnd=timeGetTime();
		printf("%s - %d/%d %d%% removed in %d ms\n",__FUNCTION__,
			triangle_count-deleted_triangles,
			triangle_count,deleted_triangles*100/triangle_count,
			timeEnd-timeStart);
		
	}

	// Check if a triangle flips when this edge is removed
	bool flipped(vec3f p,int i0,int i1,Vertex &v0,Vertex &v1,std::vector<int> &deleted)
	{
		int bordercount=0;

		//检测点p是否与v0连接的三角形中除v0外的其他两个点构成翻面
		loopk(0,v0.tcount)
		{
			Triangle &t=triangles[refs[v0.tstart+k].tid]; 
			if(t.deleted)continue; //已经标记为删除

			int s=refs[v0.tstart+k].tvertex;
			int id1=t.v[(s+1)%3];
			int id2=t.v[(s+2)%3];

			//三角形t与边{i0,i1}相连,则t将在边{i0,i1}缩边操作中被删除
			if(id1==i1 || id2==i1) // delete ?
			{
				bordercount++;
				deleted[k]=1;
				continue;
			}


			vec3f d1 = vertices[id1].p-p; d1.normalize();
			vec3f d2 = vertices[id2].p-p; d2.normalize(); 
			if(fabs(d1.dot(d2))>0.999/*小于2.85度夹角*/) return true; //如果三角形{id1,p,id2}成退化三角形(夹角太小)
			
			//新法向与t.n比较
			vec3f n;
			n.cross(d1,d2);
			n.normalize();
			deleted[k]=0;							
			if(n.dot(t.n)<0.2/*法向偏转大于78.5度*/) return true;
		}
		return false;
	}

	// Update triangle connections and edge error after a edge is collapsed
	// todo:更新顶点i0上的二次误差矩阵
	void update_triangles(int i0,Vertex &v,const std::vector<int> &deleted,int &deleted_triangles)
	{
		vec3f p;
		loopk(0,v.tcount)
		{
			//通过与v相连的one-ring corner得到one-ring三角形
			Ref &r=refs[v.tstart+k];
			Triangle &t=triangles[r.tid]; 

			//已经删除的三角形不处理
			if(t.deleted)continue;

			//将要消失的三角形不用处理
			if(deleted[k]) 
			{
				t.deleted=1; //标记删除
				deleted_triangles++;
				continue;
			}

			//将端点v所在的顶点索引替换成i0
			t.v[r.tvertex]=i0;

			//三角形t标记为dirty
			t.dirty=1;

			//重新计算三角形t各边的二次误差及其最优点
			t.err[0]=calculate_error(t.v[0],t.v[1],p);
			t.err[1]=calculate_error(t.v[1],t.v[2],p);
			t.err[2]=calculate_error(t.v[2],t.v[0],p);
			t.err[3]=min(t.err[0],min(t.err[1],t.err[2]));

			//将变化的(保留的)corner追加到refs
			refs.push_back(r);
		}
	}

	// compact triangles, compute edge error and build reference list
	void update_mesh(int iteration)
	{		
		if(iteration>0) // compact triangles
		{
			int dst=0;
			loopi(0,triangles.size())
			if(!triangles[i].deleted)
			{
				//todo:如果i==dst可以减少一次赋值操作
				//不过这种情况很少,一旦有某个三角形删除的情况下
				triangles[dst++]=triangles[i];
			}
			triangles.resize(dst);
		}
		//
		// Init Quadrics by Plane & Edge Errors
		//
		// required at the beginning ( iteration == 0 )
		// recomputing during the simplification is not required,
		// but mostly improves the result for closed meshes
		//
		/*
			通过平面以及缩边的二次误差来初始化二次型对称矩阵

			只需要在第一次迭代塌边之前收集二次误差信息及一些拓扑连接信息,在化简过程中没必要
			重新进行计算或者手机三角形边的二次误差信息;但是重新计算二次误差无疑会得到更加贴近
			原始模型的化简效果;(这需要在化简效率和化简效果之间进行平衡,例如百万级别的三角网
			化简则优先考虑效率,反之,十万级别的三角形网则优先考虑效果)
		*/
		if( iteration == 0 )
		{
			//顶点的平面清零
			loopi(0,vertices.size()) 
			vertices[i].q=SymetricMatrix(0.0);

			//将每个三角形平面累加到相连的没给顶点上
			loopi(0,triangles.size()) 
			{
				//获取三角形的顶点位置信息
				Triangle &t=triangles[i]; 
				vec3f n,p[3];
				loopj(0,3) p[j]=vertices[t.v[j]].p;
				
				//计算三角形所在平面的单位法向
				n.cross(p[1] - p[0], p[2] - p[0]);
				n.normalize();
				t.n=n; 

				//在三角形的顶点上累加平面
				loopj(0,3) vertices[t.v[j]].q = 
					vertices[t.v[j]].q+SymetricMatrix(n.x,n.y,n.z,-n.dot(p[0]));
			}

			//计算三角形每条边在塌边时的最优点及其二次误差(塌边能量)
			loopi(0,triangles.size())
			{
				// Calc Edge Error
				Triangle &t=triangles[i];vec3f p;
				loopj(0,3) t.err[j]=calculate_error(t.v[j],t.v[(j+1)%3],p);  //计算每条边的能量值
				t.err[3]=min(t.err[0],min(t.err[1],t.err[2]));               //计算三角形的能量值
			}	
		}

		// Init Reference ID list	
		loopi(0,vertices.size())
		{
			vertices[i].tstart=0;
			vertices[i].tcount=0;
		}

		//统计每个顶点所连接的三角形数量
		loopi(0,triangles.size())
		{
			Triangle &t=triangles[i];
			loopj(0,3) vertices[t.v[j]].tcount++;
		}

		//每个顶点的corner在数组中的起始索引号
		int tstart=0;
		loopi(0,vertices.size())
		{
			Vertex &v=vertices[i];
			v.tstart=tstart;
			tstart+=v.tcount;
			v.tcount=0; //顶点v的corner数量置零
		}

		// Write References
		/*记录顶点vertex与关联的corner{tid,tvertexOrder}
		收集顶点的corner属性(ref)，然后按照顶点索引顺序存储在vector中;
		每个顶点的corner个数记录在顶点v.tcount中;
		ref.tid记录corner所在三角形的索引号;
		ref.tvertex记录corner的顶点在该三角形tid中序号[0...2];
		*/
		refs.resize(triangles.size()*3);
		loopi(0,triangles.size())
		{
			//三角形
			Triangle &t=triangles[i];	
			loopj(0,3)
			{
				//corner
				Vertex &v=vertices[t.v[j]];
				refs[v.tstart+v.tcount].tid=i;     //三角形序号
				refs[v.tstart+v.tcount].tvertex=j; //顶点序号[0..2]
				v.tcount++;
			}
		}

		// Identify boundary : vertices[].border=0,1 
		//边界探测
		/*
		todo:
		1)需要增加边如果只与一个三角形相连,则视为边界,需要增加垂直平面来阻止先于内部塌边的操作;
		2)与某边相连的某两个三角之间的二面角小于5度是需要增加垂直平面来阻止先于其他塌边操作(需要
		考虑此时两个三角形如果在这条边上是非流型情况时,角度的计算问题);
		*/
		if( iteration == 0 )
		{
			std::vector<int> vcount,vids;

			loopi(0,vertices.size())
				vertices[i].border=0;

			loopi(0,vertices.size())
			{
				//顶点v
				Vertex &v=vertices[i];

				/*收集顶点v相连的corner所在三角形(顶点v的one-ring三角形)的所有
				顶点的索引编码及其出现的重数(出现的次数)*/
				vcount.clear();  //对应点的重数(multiplicity)
				vids.clear();    //顶点集合(vid set)

				//顶点v相连的corner
				loopj(0,v.tcount)
				{
					//corner对应的三角形序号
					int k=refs[v.tstart+j].tid;
					Triangle &t=triangles[k];

					//corner所在三角形的三个顶点
					loopk(0,3)
					{
						//在vids数组中查找是否含有id元素,并通过ofs下标指示具体位置
						int ofs=0,id=t.v[k];
						while(ofs<vcount.size())
						{
							if(vids[ofs]==id)break; 
							ofs++;
						}

						//在vids中没找到id,则将id追加到vids中
						if(ofs==vcount.size())
						{    
							vcount.push_back(1);
							vids.push_back(id);
						}
						//在vids中的ofs位置找到了id,那么重数加1
						else 
							vcount[ofs]++;
					}
				}

				/*只与一个三角形相连的顶点标记为border==1;
				  后续判断某条边中只有含有一个这种border==1点的边就是需要放弃塌边操作的；
				  如果两个点都是这种border==1的点,则视为某个孤立三角形(即与其他三角网只通过0或者1个顶点连通),
				  这种孤立三角形也是放弃塌边操作*/
				loopj(0,vcount.size()) if(vcount[j]==1)
					vertices[vids[j]].border=1;					
			}
		}
	}

	// Finally compact mesh before exiting
	void compact_mesh()
	{
		int dst=0;
		loopi(0,vertices.size())
		{
			vertices[i].tcount=0;
		}
		loopi(0,triangles.size())
		if(!triangles[i].deleted)
		{
			Triangle &t=triangles[i];
			triangles[dst++]=t;                   //三角形对象被替换
			loopj(0,3)vertices[t.v[j]].tcount=1;
		}
		//三角形对象内存收缩
		triangles.resize(dst);
		dst=0;
		loopi(0,vertices.size())
		if(vertices[i].tcount)
		{
			vertices[i].tstart=dst;
			vertices[dst].p=vertices[i].p;        //只替换顶点的几何位置,其他信息不动
			dst++;
		}
		loopi(0,triangles.size())
		{
			Triangle &t=triangles[i];
			loopj(0,3)t.v[j]=vertices[t.v[j]].tstart;
		}
		//顶点对象内存收缩
		vertices.resize(dst);
	}

	// Error between vertex and Quadric
	// 计算点{x,y,z}到平面{a,b,c,d}的平方距离
	double vertex_error(SymetricMatrix q, double x, double y, double z)
	{
 		return   q[0]*x*x + 2*q[1]*x*y + 2*q[2]*x*z + 2*q[3]*x + q[4]*y*y
 		     + 2*q[5]*y*z + 2*q[6]*y + q[7]*z*z + 2*q[8]*z + q[9];
	}

	// Error for one edge
	// 计算在线段{id_v1,id_v2}间某一点p_result
	// 到连接在点{id_v1}和点{id_v2}上所有平面的最短距离平方和最小
	double calculate_error(int id_v1, int id_v2, vec3f &p_result)
	{
		// compute interpolated vertex 
		// 收集点v1,v2的所有二次误差矩阵
		SymetricMatrix q = vertices[id_v1].q + vertices[id_v2].q;

		// v1,v2中只要有一个是边界点(改点只与一个三角形连接)则边{v1,v2}为border
		bool   border = vertices[id_v1].border & vertices[id_v2].border;
		double error=0;

		//计算4*4矩阵q的3*3子矩阵的行列式
		double det = q.det(0, 1, 2, 1, 4, 5, 2, 5, 7);

		if ( det != 0 && !border )
		{
			// q_delta is invertible(克莱姆法则求解非齐次线性方程组,详见Simplify.GL-SymetricMatrix.png之推导)
			p_result.x = -1/det*(q.det(1, 2, 3, 4, 5, 6, 5, 7 , 8));	// vx = A41/det(q_delta) 
			p_result.y =  1/det*(q.det(0, 2, 3, 1, 5, 6, 2, 7 , 8));	// vy = A42/det(q_delta) 
			p_result.z = -1/det*(q.det(0, 1, 3, 1, 4, 6, 2, 5,  8));	// vz = A43/det(q_delta) 
			error = vertex_error(q, p_result.x, p_result.y, p_result.z);
		}
		else
		{
			// det = 0 -> try to find best result
			// 如果q行或者列线性相关(任何一子矩阵行列式线性相关或者行列式为零)或者border==true,
			// 取两个端点和中点中距离平方和最小者为最优点
			vec3f p1=vertices[id_v1].p;
			vec3f p2=vertices[id_v2].p;
			vec3f p3=(p1+p2)/2;

			// 分别计算距离平方和
			double error1 = vertex_error(q, p1.x,p1.y,p1.z);
			double error2 = vertex_error(q, p2.x,p2.y,p2.z);
			double error3 = vertex_error(q, p3.x,p3.y,p3.z);

			// 取距离平方和最小值
			error = min(error1, min(error2, error3));

			// 返回距离平方和最小的点{v1,v2,中点}
			if (error1 == error) p_result=p1;
			if (error2 == error) p_result=p2;
			if (error3 == error) p_result=p3;
		}
		return error;
	}

	// Optional : Store as OBJ
	void write_obj(char* filename)
	{
		FILE *file=fopen(filename, "w");
		if (!file)
		{
			printf("write_obj: can't write data file \"%s\".\n", filename);
			system("PAUSE");
			exit(0);
		}
		//输出顶点坐标信息
		loopi(0,vertices.size())
		{
			fprintf(file, "v %lf %lf %lf\n", vertices[i].p.x,vertices[i].p.y,vertices[i].p.z);
		}	
		//输出三角形网格索引信息
		loopi(0,triangles.size()) if(!triangles[i].deleted)
		{
			fprintf(file, "f %d// %d// %d//\n", triangles[i].v[0]+1, triangles[i].v[1]+1, triangles[i].v[2]+1);
		}
		fclose(file);
	}
};
///////////////////////////////////////////
